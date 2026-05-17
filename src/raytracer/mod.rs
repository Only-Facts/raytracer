use std::{
    fs::File,
    io::{BufReader, Read},
    path::PathBuf,
    str::FromStr,
    sync::{
        Arc, Mutex,
        atomic::{AtomicBool, AtomicUsize, Ordering},
    },
    thread,
    time::Duration,
};

use sfml::{graphics::RenderWindow, window::Style};

use crate::{
    Error,
    config::loader::PluginLoader,
    ffi::bridge::{LightBridge, ObjectBridge},
    print_progress_bar,
    raytracer::{
        camera::Camera,
        gui::{InputState, SharedInput, SharedPixels, apply_input, loop_window_threaded},
        ray::Ray,
        structs::Color,
    },
    utils::vector::Vector3,
};

pub mod camera;
pub mod gui;
pub mod objects;
pub mod ray;
pub mod structs;

const PPM_MAGIC: u8 = 22;

fn merge_color(color1: Color, color2: Color, intensity: f32) -> Color {
    let new_color = Vector3::new(
        color1.x as f32 + color2.x as f32 * intensity,
        color1.y as f32 + color2.y as f32 * intensity,
        color1.z as f32 + color2.z as f32 * intensity,
    );

    Color::new(
        new_color.x.min(255.0) as u8,
        new_color.y.min(255.0) as u8,
        new_color.z.min(255.0) as u8,
    )
}

fn process_camera_chunk(
    rays: &mut [Ray],
    render_distance: f64,
    sky_color: Color,
    objects: &[ObjectBridge],
    lights: &[LightBridge],
    progress: Option<Arc<AtomicUsize>>,
) {
    let sdf_colliding_limit = 1e-2;

    for ray in rays.iter_mut() {
        while ray.base.alive {
            if ray.base.cframe.orientation.length() <= 1e-8 {
                ray.base.kill();
                continue;
            }

            let mut min_sdf = f64::MAX;
            let mut nearest_object = None;
            let mut face_hit: *const std::ffi::c_void = std::ptr::null();

            for object in objects {
                if ray
                    .base
                    .potential_objects
                    .contains(&(object.instance as usize))
                {
                    continue;
                }

                let actual_distance;
                let mut actual_face_ptr: *const std::ffi::c_void = std::ptr::null();

                if object.obj_type == "sphere" {
                    actual_distance =
                        (ray.base.cframe.position - object.position).length() - object.radius;
                } else if object.use_aabb {
                    let p = ray.base.cframe.position;
                    let c = object.aabb_center;
                    let e = object.aabb_extents;

                    let dx = (p.x - c.x).abs() - e.x;
                    let dy = (p.y - c.y).abs() - e.y;
                    let dz = (p.z - c.z).abs() - e.z;

                    let max_d = dx.max(dy).max(dy);

                    if max_d > 0.05 {
                        let out_dist =
                            dx.max(0.0).powi(2) + dy.max(0.0).powi(2) + dz.max(0.0).powi(2);
                        actual_distance = out_dist.sqrt() + max_d.min(0.0);
                    } else {
                        let res = object.compute_sdf(ray.base.cframe.position);
                        actual_distance = res.distance;
                        actual_face_ptr = res.face_ptr;
                    }
                } else {
                    let res = object.compute_sdf(ray.base.cframe.position);
                    actual_distance = res.distance;
                    actual_face_ptr = res.face_ptr;
                }

                if actual_distance < min_sdf {
                    min_sdf = actual_distance;
                    nearest_object = Some(object);
                    face_hit = actual_face_ptr;
                }
            }

            if nearest_object.is_none() || min_sdf == f64::MAX {
                ray.color = merge_color(ray.color, sky_color, ray.coef);
                ray.base.kill();
                continue;
            }

            let nearest = nearest_object.unwrap();

            if min_sdf < sdf_colliding_limit {
                let normal = if nearest.obj_type == "sphere" {
                    (ray.base.cframe.position - nearest.position).normalize()
                } else {
                    nearest
                        .compute_hit(ray.base.cframe.position, face_hit)
                        .normalize()
                };

                if nearest.is_mirror() {
                    let dot = ray.base.cframe.orientation.dot(normal);
                    ray.base.cframe.orientation -= normal * (2.0 * dot);
                    ray.base.cframe.position +=
                        ray.base.cframe.orientation * (sdf_colliding_limit * 2.0);
                    ray.base.potential_objects.clear();
                    ray.base.potential_objects.push(nearest.instance as usize);
                } else {
                    let view_dir = ray.base.cframe.orientation * -1.0;
                    let base_obj_color_res = nearest.get_point_color(ray.base.cframe.position);
                    let base_obj_color = Color::new(
                        base_obj_color_res.r,
                        base_obj_color_res.g,
                        base_obj_color_res.b,
                    );

                    let mut total_light_r = 0.0;
                    let mut total_light_g = 0.0;
                    let mut total_light_b = 0.0;

                    for light in lights {
                        let light_pos = light.get_position();
                        let dir_to_light = (light_pos - ray.base.cframe.position).normalize();
                        let dist_to_light = (light_pos - ray.base.cframe.position).length();

                        let mut shadow_factor: f64 = 1.0;
                        let shadow_origin =
                            ray.base.cframe.position + (normal * (sdf_colliding_limit * 2.0));

                        let k = 16.0;
                        let mut t = sdf_colliding_limit;

                        while t < dist_to_light {
                            let current_shadow_pos = shadow_origin + dir_to_light * t;
                            let mut min_obstacle_dist = f64::MAX;

                            for obstacle in objects {
                                if obstacle.instance == nearest.instance {
                                    continue;
                                }

                                let shadow_res_distance = if obstacle.obj_type == "sphere" {
                                    (current_shadow_pos - obstacle.position).length()
                                        - obstacle.radius
                                } else {
                                    obstacle.compute_sdf(current_shadow_pos).distance
                                };

                                if shadow_res_distance < min_obstacle_dist {
                                    min_obstacle_dist = shadow_res_distance;
                                }
                            }

                            if min_obstacle_dist < 1e-3 {
                                shadow_factor = 0.0;
                                break;
                            }

                            shadow_factor = shadow_factor.min(k * min_obstacle_dist / t);

                            t += min_obstacle_dist;

                            if shadow_factor <= 0.0 {
                                break;
                            }
                        }

                        shadow_factor = shadow_factor.clamp(0.0, 1.0);

                        if shadow_factor > 0.0 {
                            let dot_light = normal.dot(dir_to_light).max(0.0);
                            let (l_color, l_intensity) = light.get_color_info();

                            let mut specular_factor = 0.0;
                            if dot_light > 0.0 {
                                let half_vector = (dir_to_light + view_dir).normalize();
                                let spec_angle = normal.dot(half_vector).max(0.0);

                                let shininess = 32.0;
                                specular_factor = spec_angle.powf(shininess);
                            }

                            let specular_strength = 0.5;

                            let diffuse_r =
                                base_obj_color.x as f64 * (l_color.x as f64 / 255.0) * dot_light;
                            let diffuse_g =
                                base_obj_color.y as f64 * (l_color.y as f64 / 255.0) * dot_light;
                            let diffuse_b =
                                base_obj_color.z as f64 * (l_color.z as f64 / 255.0) * dot_light;

                            let spec_r = 255.0
                                * (l_color.x as f64 / 255.0)
                                * specular_factor
                                * specular_strength;
                            let spec_g = 255.0
                                * (l_color.y as f64 / 255.0)
                                * specular_factor
                                * specular_strength;
                            let spec_b = 255.0
                                * (l_color.z as f64 / 255.0)
                                * specular_factor
                                * specular_strength;

                            total_light_r += (diffuse_r + spec_r) * l_intensity * shadow_factor;
                            total_light_g += (diffuse_g + spec_g) * l_intensity * shadow_factor;
                            total_light_b += (diffuse_b + spec_b) * l_intensity * shadow_factor;
                        }
                    }

                    let final_r =
                        (total_light_r + (base_obj_color.x as f64 * 0.3)).min(255.0) as u8;
                    let final_g =
                        (total_light_g + (base_obj_color.y as f64 * 0.3)).min(255.0) as u8;
                    let final_b =
                        (total_light_b + (base_obj_color.z as f64 * 0.3)).min(255.0) as u8;

                    let surface_color = Color::new(final_r, final_g, final_b);

                    ray.color = merge_color(ray.color, surface_color, ray.coef);
                    ray.base.kill();
                }
                continue;
            }

            let translation = ray.base.cframe.orientation * min_sdf;
            ray.base.cframe.position += translation;
            ray.base.distance += min_sdf;

            if ray.base.distance >= render_distance * 2.0 {
                ray.color = merge_color(ray.color, sky_color, ray.coef);
                ray.base.kill();
                continue;
            }
        }

        if let Some(ref counter) = progress {
            counter.fetch_add(1, Ordering::SeqCst);
        }
    }
}

#[allow(dead_code)]
#[derive(Debug, Clone)]
pub struct Settings {
    pub viewer: bool,
    pub ppm_path: String,
    pub cfg_path: String,
    pub gui: bool,
    pub debug: bool,
    pub adv: bool,
    pub newton: bool,
    pub camera_path: PathBuf,
    pub plugins_path: PathBuf,
    pub rendered_path: PathBuf,
    pub obj_path: PathBuf,
    pub resolution: (u32, u32),
    pub nproc_set: bool,
    pub nproc: usize,
    pub camera_set: bool,
    pub plugins_set: bool,
    pub rendered_set: bool,
    pub obj_set: bool,
    pub resolution_set: bool,
}

impl Default for Settings {
    fn default() -> Self {
        Self {
            viewer: false,
            ppm_path: String::new(),
            cfg_path: String::new(),
            gui: false,
            debug: false,
            adv: false,
            newton: false,
            camera_path: PathBuf::new(),
            plugins_path: PathBuf::from_str("./plugins").unwrap_or_default(),
            rendered_path: PathBuf::from_str("./rendered").unwrap_or_default(),
            obj_path: PathBuf::from_str("./obj").unwrap_or_default(),
            resolution: (0, 0),
            nproc_set: false,
            nproc: 0,
            camera_set: false,
            plugins_set: false,
            rendered_set: false,
            obj_set: false,
            resolution_set: false,
        }
    }
}

#[allow(dead_code)]
pub trait Factory<T> {
    fn factory(&self, name: &str) -> &T;
}

#[derive(Default, Clone)]
pub struct Raytracer {
    pub settings: Settings,
    pub camera: camera::Viewer,
    pub objects: Vec<ObjectBridge>,
    pub lights: Vec<LightBridge>,
    pub plugins: Option<PluginLoader>,
}

#[allow(dead_code)]
impl Raytracer {
    pub fn new(
        camera: camera::Viewer,
        objects: Vec<ObjectBridge>,
        lights: Vec<LightBridge>,
    ) -> Self {
        Self {
            settings: Settings::default(),
            objects,
            lights,
            camera,
            plugins: None,
        }
    }

    pub fn load_render(&mut self) -> Result<(), Error> {
        let path = &self.settings.ppm_path;

        let file =
            File::open(path).map_err(|_| format!("Failed to open file for reading: {}", path))?;

        let mut reader = BufReader::new(file);

        let mut magic_buf = [0u8; 1];
        let mut width_buf = [0u8; 2];
        let mut height_buf = [0u8; 2];

        reader.read_exact(&mut magic_buf)?;
        reader.read_exact(&mut width_buf)?;
        reader.read_exact(&mut height_buf)?;

        let magic = magic_buf[0];
        let width = u16::from_ne_bytes(width_buf);
        let height = u16::from_ne_bytes(height_buf);

        if magic != PPM_MAGIC {
            return Err(format!("Invalid magic number in file: {}", path).into());
        }

        let buffer_size = width as usize * height as usize * 3;
        let mut buffer = vec![0u8; buffer_size];

        reader
            .read_exact(&mut buffer)
            .map_err(|_| format!("Error during the file reading: {}", path))?;

        let rays = self.camera.get_rays_mut();

        for (i, ray) in rays.iter_mut().enumerate() {
            let r = buffer[i * 3];
            let g = buffer[i * 3 + 1];
            let b = buffer[i * 3 + 2];

            ray.set_color(Color::new(r, g, b));
        }

        Ok(())
    }

    fn render_thread_loop(
        &mut self,
        shared_pixels: SharedPixels,
        shared_input: SharedInput,
        running: Arc<AtomicBool>,
        width: usize,
        height: usize,
        show_progress: bool,
    ) {
        while running.load(Ordering::Relaxed) {
            let input = {
                let input = shared_input.lock().expect("shared input mutex poisoned");

                *input
            };

            apply_input(&mut self.camera, input);

            self.render_once(show_progress);

            self.camera.update_screen();

            let screen = self.camera.get_screen();

            {
                let mut pixels = shared_pixels
                    .lock()
                    .expect("shared pixel buffer mutex poisoned");

                let expected_len = width * height;

                if pixels.len() != expected_len {
                    pixels.resize(expected_len, Color::default());
                }

                let copy_len = pixels.len().min(screen.len());

                pixels[..copy_len].copy_from_slice(&screen[..copy_len]);
            }

            thread::sleep(Duration::from_millis(1));
        }
    }

    pub fn render_once(&mut self, show_progress: bool) {
        self.camera.reset();

        let rays = self.camera.get_rays_mut();

        if rays.is_empty() {
            return;
        }

        let total_rays = rays.len();
        let progress_counter = Arc::new(AtomicUsize::new(0));
        let num_threads = if self.settings.nproc_set {
            self.settings.nproc
        } else {
            thread::available_parallelism()
                .map(|n| n.get())
                .unwrap_or(1)
        };

        let chunk_size = rays.len().div_ceil(num_threads);

        let render_distance = 400.0;
        let sky_color = Color::new(135, 206, 235);

        let objects_ref = &self.objects;
        let lights_ref = &self.lights;

        thread::scope(|scope| {
            if show_progress {
                let progress_ref = Arc::clone(&progress_counter);
                scope.spawn(move || {
                    while progress_ref.load(Ordering::Relaxed) < total_rays {
                        print_progress_bar(
                            "Rendering:",
                            progress_ref.load(Ordering::Relaxed),
                            total_rays,
                        );
                        thread::sleep(Duration::from_millis(100));
                    }
                    print_progress_bar("Rendering:", total_rays, total_rays);
                });
            }
            for chunk in rays.chunks_mut(chunk_size) {
                let progress_ref = Arc::clone(&progress_counter);
                scope.spawn(move || {
                    process_camera_chunk(
                        chunk,
                        render_distance,
                        sky_color,
                        objects_ref,
                        lights_ref,
                        Some(progress_ref),
                    );
                });
            }
        });
    }

    pub fn gui(&mut self) -> Result<(), Error> {
        if self.settings.viewer {
            self.load_render()?;
        }

        let resolution = self.camera.get_resolution();

        let width = resolution.x as usize;
        let height = resolution.y as usize;

        let shared_pixels: SharedPixels =
            Arc::new(Mutex::new(vec![Color::default(); width * height]));
        let shared_input: SharedInput = Arc::new(Mutex::new(InputState::default()));

        let running = Arc::new(AtomicBool::new(true));

        if self.settings.viewer {
            self.camera.update_screen();

            let screen = self.camera.get_screen();

            let mut pixels = shared_pixels
                .lock()
                .expect("shared pixel buffer mutex poisoned");

            let copy_len = pixels.len().min(screen.len());
            pixels[..copy_len].copy_from_slice(&screen[..copy_len]);
        }

        let render_pixels = Arc::clone(&shared_pixels);
        let render_input = Arc::clone(&shared_input);
        let render_running = Arc::clone(&running);
        let gui_enabled = self.settings.gui;

        let show_progress = self.settings.adv;

        let mut render_state = self.clone();

        let render_handle = thread::spawn(move || {
            if gui_enabled {
                render_state.render_thread_loop(
                    render_pixels,
                    render_input,
                    render_running,
                    width,
                    height,
                    show_progress,
                );
            }
        });

        let mut window = RenderWindow::new(
            (resolution.x as u32, resolution.y as u32),
            "Raytracer",
            Style::TITLEBAR | Style::CLOSE,
            &Default::default(),
        )?;

        window.set_vertical_sync_enabled(true);

        loop_window_threaded(
            &mut window,
            shared_pixels,
            shared_input,
            Arc::clone(&running),
            width,
            height,
        );

        running.store(false, Ordering::Relaxed);

        if render_handle.join().is_err() {
            eprintln!("Render thread panicked");
        }

        window.close();

        Ok(())
    }

    pub fn parse_flags(&mut self, args: Vec<String>) {
        let mut i = 1;

        while i < args.len() {
            match args[i].as_str() {
                "-gui" => {
                    self.settings.gui = true;
                }
                "-a" | "--advencement" => {
                    self.settings.adv = true;
                }
                "-n" | "--nproc" => {
                    if let Some(nproc_str) = args.get(i + 1)
                        && let Ok(n) = nproc_str.parse::<usize>()
                    {
                        self.settings.nproc = n;
                        self.settings.nproc_set = true;
                        i += 1;
                    }
                }
                "-g" | "--newton" => {
                    self.settings.newton = true;
                }
                "-c" | "--camera" => {
                    if let Some(path) = args.get(i + 1) {
                        self.settings.camera_path = PathBuf::from(path);
                        self.settings.camera_set = true;
                        i += 1;
                    }
                }
                "-p" | "--plugins" => {
                    if let Some(path) = args.get(i + 1) {
                        self.settings.plugins_path = PathBuf::from(path);
                        self.settings.plugins_set = true;
                        i += 1;
                    }
                }
                "-o" | "--obj" => {
                    if let Some(path) = args.get(i + 1) {
                        self.settings.obj_path = PathBuf::from(path);
                        self.settings.obj_set = true;
                        i += 1;
                    }
                }
                "-s" | "--save" => {
                    if let Some(path) = args.get(i + 1) {
                        self.settings.rendered_path = PathBuf::from(path);
                        self.settings.rendered_set = true;
                        i += 1;
                    }
                }
                "-r" | "--resolution" => {
                    if let Some(res_str) = args.get(i + 1) {
                        let parts: Vec<&str> = res_str.split('x').collect();
                        if parts.len() == 2
                            && let (Ok(w), Ok(h)) =
                                (parts[0].parse::<u32>(), parts[1].parse::<u32>())
                        {
                            self.settings.resolution = (w, h);
                            self.settings.resolution_set = true;
                        }
                        i += 1;
                    }
                }
                s => {
                    if !s.starts_with('-') {
                        if s.ends_with(".ppm") {
                            self.settings.ppm_path = s.to_string();
                            self.settings.viewer = true;
                        } else {
                            self.settings.cfg_path = s.to_string();
                        }
                    }
                }
            }
            i += 1;
        }
    }
}
