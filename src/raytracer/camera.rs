use std::{collections::HashMap, sync::OnceLock};

use tokio::sync::Mutex;

use crate::raytracer::{
    objects::{ChunkLightData, Object, ObjectDescriptor},
    ray::{Ray, RayBase},
    structs::{CFrame, Color, Coord, Direction, Face},
};

pub trait Camera: Object {
    fn init(&mut self);
    fn reset(&mut self);
    fn update_screen(&mut self);
    fn get_screen(&self) -> &Vec<Color>;
    fn get_rays(&self) -> &Vec<Ray>;
    fn get_rays_mut(&mut self) -> &mut Vec<Ray>;
}

#[derive(Default)]
pub struct Viewer {
    pub descriptor: ObjectDescriptor,
    pub resolution: (u32, u32),
    pub fov: f64,
    pub screen: Vec<Color>,
    pub rays: Vec<Ray>,
    pub immunity: Option<Box<dyn Object>>,
}

impl Viewer {
    pub fn new(pos: Coord, dir: Direction, res_x: u32, res_y: u32, fov: f64) -> Self {
        let mut descriptor = ObjectDescriptor::default();
        descriptor.cframe.position = pos;
        descriptor.cframe.orientation = dir;

        let mut viewer = Self {
            descriptor,
            resolution: (res_x, res_y),
            fov,
            screen: vec![Color::default(); (res_x * res_y) as usize],
            rays: Vec::new(),
            immunity: None,
        };
        viewer.init();
        viewer
    }
}

impl Camera for Viewer {
    fn init(&mut self) {
        let size = (self.resolution.0 * self.resolution.1) as usize;
        self.screen.clear();
        self.screen.resize(size, Color::default());

        self.rays.clear();
        for _ in 0..size {
            let base = RayBase::new(self.descriptor.cframe.clone());
            self.rays.push(Ray {
                base,
                color: Color::default(),
                coef: 1.0,
            });
        }
        self.reset();
    }

    fn reset(&mut self) {
        let (width, height) = (self.resolution.0 as f64, self.resolution.1 as f64);
        let aspect_ratio = width / height;

        let fov_rad = self.fov.to_radians();
        let scale = (fov_rad * 0.5).tan();

        let forward = self.descriptor.cframe.orientation.normalize();

        let tmp_up = Coord::new(0.0, 1.0, 0.0);
        let right = forward.cross(tmp_up).normalize();
        let up = right.cross(forward).normalize();

        for y in 0..self.resolution.1 {
            for x in 0..self.resolution.0 {
                let i = (y * self.resolution.0 + x) as usize;

                let px = (2.0 * (x as f64 + 0.5) / width - 1.0) * aspect_ratio * scale;
                let py = (1.0 - 2.0 * (y as f64 + 0.5) / height) * scale;

                let direction = (forward + (right * px) + (up * py)).normalize();

                let ray = &mut self.rays[i];
                ray.reset();
                ray.base.cframe.position = self.descriptor.cframe.position;
                ray.base.cframe.orientation = direction;
                ray.color = Color::default();
                ray.coef = 1.0;
            }
        }
    }

    fn update_screen(&mut self) {
        for (i, ray) in self.rays.iter().enumerate() {
            if !ray.base.alive {
                self.screen[i] = ray.color;
            }
        }
    }

    fn get_screen(&self) -> &Vec<Color> {
        &self.screen
    }

    fn get_rays(&self) -> &Vec<Ray> {
        &self.rays
    }

    fn get_rays_mut(&mut self) -> &mut Vec<Ray> {
        &mut self.rays
    }
}

impl Object for Viewer {
    fn get_descriptor(&self) -> &ObjectDescriptor {
        &self.descriptor
    }

    fn will_collide(&self, _point: &Coord, _dir: &Direction) -> bool {
        false
    }

    fn translate(&mut self, v: &Coord) {
        self.descriptor.cframe.position += *v;
        self.reset();
    }

    fn rotate(&mut self, v: &Direction) {
        self.descriptor.cframe.orientation += *v;
        self.reset();
    }

    fn get_cframe(&self) -> &CFrame {
        &self.descriptor.cframe
    }

    fn parse(&mut self, _node: &serde_json::Value) {}
    fn load_obj(&mut self, _path: &str) {}
    fn reflect_ray(&self, _ray: &mut Ray, _face: &Face) {}
    fn compute_sdf(&self, _point: &Coord) -> (f32, &Face) {
        static EMPTY_VEC: OnceLock<Face> = OnceLock::new();
        let empty_face = EMPTY_VEC.get_or_init(|| Vec::new());
        (f32::MAX, empty_face)
    }
    fn compute_hit(&self, _point: &Coord, _face: Option<&Face>) -> Coord {
        Coord::new(0.0, 0.0, 0.0)
    }
    fn set_immunity(&mut self, object: Option<Box<dyn Object>>) {
        self.immunity = object;
    }
}
