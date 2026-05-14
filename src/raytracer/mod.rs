use std::{
    fs::File,
    io::{BufReader, Read},
    path::PathBuf,
    str::FromStr,
    sync::{
        atomic::{AtomicBool, Ordering},
        Arc, Mutex,
    },
    thread,
    time::Duration,
};

use sfml::{
    graphics::{
        Color as SfColor, PrimitiveType, RenderStates, RenderTarget, RenderWindow, Vertex,
    },
    system::Vector2f,
    window::{Event, Key, Style},
};

use crate::{
    ffi::bridge::{LightBridge, ObjectBridge},
    raytracer::{camera::Camera, ray::Ray, structs::Color},
    utils::vector::{Vector2, Vector3},
    Error,
};

pub mod camera;
pub mod objects;
pub mod ray;
pub mod structs;

const PPM_MAGIC: u8 = 22;

type SharedPixels = Arc<Mutex<Vec<Color>>>;

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
    _global_light_color: Color,
    _global_light_count: usize,
    objects: &[ObjectBridge],
    lights: &[LightBridge],
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
                let actual_sdf = object.compute_sdf(ray.base.cframe.position);
                if actual_sdf.distance < min_sdf {
                    min_sdf = actual_sdf.distance;
                    nearest_object = Some(object);
                    face_hit = actual_sdf.face_ptr;
                }
            }

            if nearest_object.is_none() || min_sdf == f64::MAX {
                ray.color = merge_color(ray.color, sky_color, ray.coef);
                ray.base.kill();
                continue;
            }

            let nearest = nearest_object.unwrap();

            let translation = ray.base.cframe.orientation * min_sdf;
            ray.base.cframe.position += translation;
            ray.base.distance += min_sdf;

            if ray.base.distance >= render_distance * 2.0 {
                ray.base.kill();
                continue;
            }

            if min_sdf < sdf_colliding_limit {
                let normal = nearest.compute_hit(translation, face_hit).normalize();

                if nearest.is_mirror() {
                    let dot = ray.base.cframe.orientation.dot(normal);
                    ray.base.cframe.orientation =
                        ray.base.cframe.orientation - normal * (2.0 * dot);
                    ray.base.cframe.position +=
                        ray.base.cframe.orientation * (sdf_colliding_limit * 2.0);
                    ray.base.potential_objects.clear();
                    ray.base.potential_objects.push(nearest.instance as usize);
                } else {
                    let mut final_pixel_color = Color::default();
                    let base_obj_color_res = nearest.get_point_color(ray.base.cframe.position);
                    let base_obj_color = Color::new(
                        base_obj_color_res.r,
                        base_obj_color_res.g,
                        base_obj_color_res.b,
                    );

                    for light in lights {
                        let light_pos = light.get_position();
                        let dir_to_light = (light_pos - ray.base.cframe.position).normalize();
                        let dist_to_light = (light_pos - ray.base.cframe.position).length();

                        let mut shadow_factor = 1.0;
                        for obstacle in objects {
                            let shadow_res = obstacle.compute_sdf(
                                ray.base.cframe.position + (normal * (sdf_colliding_limit * 2.0)),
                            );
                            if shadow_res.distance < dist_to_light {
                                shadow_factor = 0.0;
                                break;
                            }
                        }

                        if shadow_factor > 0.0 {
                            let dot_light = normal.dot(dir_to_light).max(0.0);
                            let (l_color, l_intensity) = light.get_color_info();

                            let light_contribution = Color::new(
                                (base_obj_color.x as f64
                                    * (l_color.x as f64 / 255.0)
                                    * dot_light
                                    * l_intensity) as u8,
                                (base_obj_color.y as f64
                                    * (l_color.y as f64 / 255.0)
                                    * dot_light
                                    * l_intensity) as u8,
                                (base_obj_color.z as f64
                                    * (l_color.z as f64 / 255.0)
                                    * dot_light
                                    * l_intensity) as u8,
                            );
                            final_pixel_color =
                                merge_color(final_pixel_color, light_contribution, 1.0);
                        }
                    }

                    let ambient = Color::new(
                        (base_obj_color.x as f32 * 0.1) as u8,
                        (base_obj_color.y as f32 * 0.1) as u8,
                        (base_obj_color.z as f32 * 0.1) as u8,
                    );

                    ray.color = merge_color(final_pixel_color, ambient, ray.coef);
                    ray.base.kill();
                }
            }
        }
    }
}

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
    pub resolution: Vector2<u16>,

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
            plugins_path: PathBuf::from_str("./plugins/").unwrap_or_default(),
            rendered_path: PathBuf::from_str("./rendered/").unwrap_or_default(),
            obj_path: PathBuf::from_str("./obj/").unwrap_or_default(),
            resolution: Vector2 { x: 0, y: 0 },

            camera_set: false,
            plugins_set: false,
            rendered_set: false,
            obj_set: false,
            resolution_set: false,
        }
    }
}

pub trait Factory<T> {
    fn factory(&self, name: &str) -> &T;
}

#[derive(Default)]
pub struct Raytracer {
    settings: Settings,

    step: u8,
    adv: Mutex<usize>,
    adv_max: usize,

    camera: camera::Viewer,

    objects: Vec<ObjectBridge>,
    lights: Vec<LightBridge>,
}

struct RenderState {
    camera: camera::Viewer,
    objects: Vec<ObjectBridge>,
    lights: Vec<LightBridge>,
}

impl RenderState {
    fn render_thread_loop(
        &mut self,
        shared_pixels: SharedPixels,
        running: Arc<AtomicBool>,
        width: usize,
        height: usize,
    ) {
        while running.load(Ordering::Relaxed) {
            self.render_once();

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

    fn render_once(&mut self) {
        self.camera.reset();

        let mut global_light_color = Color::new(0, 0, 0);
        let mut global_light_count = 0;

        for light in &self.lights {
            if light.is_global() {
                let (light_color, _intensity) = light.get_color_info();

                global_light_color = merge_color(global_light_color, light_color, 1.0);
                global_light_count += 1;
            }
        }

        let num_threads = thread::available_parallelism()
            .map(|n| n.get())
            .unwrap_or(1);

        let rays = self.camera.get_rays_mut();

        if rays.is_empty() {
            return;
        }

        let chunk_size = (rays.len() + num_threads - 1) / num_threads;

        let render_distance = 400.0;
        let sky_color = Color::new(135, 206, 235);

        let objects_ref = &self.objects;
        let lights_ref = &self.lights;

        thread::scope(|scope| {
            for chunk in rays.chunks_mut(chunk_size) {
                scope.spawn(move || {
                    process_camera_chunk(
                        chunk,
                        render_distance,
                        sky_color,
                        global_light_color,
                        global_light_count,
                        objects_ref,
                        lights_ref,
                    );
                });
            }
        });
    }
}

impl Raytracer {
    pub fn new(
        camera: camera::Viewer,
        objects: Vec<ObjectBridge>,
        lights: Vec<LightBridge>,
    ) -> Self {
        Self {
            settings: Settings::default(),
            step: 0,
            adv: Mutex::new(0),
            adv_max: 0,
            objects,
            lights,
            camera,
        }
    }
    pub async fn load_render(&mut self) -> Result<(), Error> {
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

        /*self.camera.set_resolution(width as u32, height as u32);
        self.camera.init();
        self.camera.kill();*/

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

    pub fn render(&mut self) {
        let mut render_state = self.take_render_state();
        render_state.render_once();

        self.camera = render_state.camera;
        self.objects = render_state.objects;
        self.lights = render_state.lights;
    }

    pub async fn gui(&mut self) -> Result<(), Error> {
        if self.settings.viewer {
            self.load_render().await?;
        } else {
            self.light()?;
        }

        let resolution = self.camera.get_resolution();

        let width = resolution.x as usize;
        let height = resolution.y as usize;

        let shared_pixels: SharedPixels =
            Arc::new(Mutex::new(vec![Color::default(); width * height]));

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

        let mut render_state = self.take_render_state();

        let render_pixels = Arc::clone(&shared_pixels);
        let render_running = Arc::clone(&running);
        let gui_enabled = self.settings.gui;

        let render_handle = thread::spawn(move || {
            if gui_enabled {
                render_state.render_thread_loop(render_pixels, render_running, width, height);
            }
        });

        let mut window = RenderWindow::new(
            (resolution.x as u32, resolution.y as u32),
            "Raytracer",
            Style::TITLEBAR | Style::CLOSE,
            &Default::default(),
        )?;

        window.set_vertical_sync_enabled(true);

        Self::loop_window_threaded(
            &mut window,
            shared_pixels,
            Arc::clone(&running),
            width,
            height,
        );

        running.store(false, Ordering::Relaxed);

        if render_handle.join().is_err() {
            eprintln!("Render thread panicked");
        }

        window.close();

        self.adv_end();

        Ok(())
    }

    fn take_render_state(&mut self) -> RenderState {
        RenderState {
            camera: std::mem::take(&mut self.camera),
            objects: std::mem::take(&mut self.objects),
            lights: std::mem::take(&mut self.lights),
        }
    }

    fn loop_window_threaded(
        window: &mut RenderWindow,
        shared_pixels: SharedPixels,
        running: Arc<AtomicBool>,
        width: usize,
        height: usize,
    ) {
        while window.is_open() {
            while let Some(event) = window.poll_event() {
                match event {
                    Event::Closed
                    | Event::KeyPressed {
                        code: Key::Escape, ..
                    } => {
                        window.close();
                    }
                    _ => {}
                }
            }

            let pixels_snapshot = {
                let pixels = shared_pixels
                    .lock()
                    .expect("shared pixel buffer mutex poisoned");

                pixels.clone()
            };

            Self::draw_pixels(window, &pixels_snapshot, width, height);
        }

        running.store(false, Ordering::Relaxed);
    }

    fn draw_pixels(window: &mut RenderWindow, pixels: &[Color], width: usize, height: usize) {
        let mut vertices = Vec::with_capacity(width * height);

        for y in 0..height {
            for x in 0..width {
                let index = y * width + x;

                let Some(color) = pixels.get(index) else {
                    continue;
                };

                let position = Vector2f::new(x as f32, y as f32);
                let sfml_color = SfColor::rgb(color.x, color.y, color.z);

                vertices.push(Vertex::with_pos_color(position, sfml_color));
            }
        }

        window.clear(SfColor::BLACK);

        window.draw_primitives(
            &vertices,
            PrimitiveType::POINTS,
            &RenderStates::default(),
        );

        window.display();
    }

    pub fn light(&mut self) -> Result<(), Error> {
        // TODO: port real C++ Raytracer::light()
        Ok(())
    }

    pub fn adv_end(&self) {
        if self.settings.adv {
            println!();
        }
    }
    /*
        These methods are referenced above but not included in your pasted file.

        Keep your real implementations if they already exist elsewhere.
        These stubs are here only so you understand what signatures are expected.

        pub fn light(&mut self) -> Result<(), Error> {
            Ok(())
        }

        pub fn adv_end(&self) {}
    */
}

/*
impl Factory<T> for Raytracer<T> {
    fn factory(&self, name: &str) -> &T {
        for lib in self.libs.iter() {

        }
    }
}
*/
