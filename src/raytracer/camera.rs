use std::collections::HashMap;

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
    lock: Mutex<()>,
    immunity: Option<Box<dyn Object>>,
    light_data: HashMap<u64, Vec<ChunkLightData>>,
    descriptor: ObjectDescriptor,

    screen: Vec<Color>,
    rays: Vec<Ray>,
    resolution: (u32, u32),
    render_distance: f32,
}

impl Viewer {
    pub fn new(res: (u32, u32)) -> Self {
        Self {
            lock: Mutex::new(()),
            immunity: None,
            light_data: HashMap::new(),
            descriptor: ObjectDescriptor::default(),
            screen: vec![Color::default(); (res.0 * res.1) as usize],
            rays: Vec::new(),
            resolution: res,
            render_distance: 1000.0,
        }
    }
}

impl Object for Viewer {
    fn get_descriptor(&self) -> &ObjectDescriptor {
        &self.descriptor
    }

    fn will_collide(&self, point: &Coord, dir: &Direction) -> bool {
        false
    }

    fn translate(&mut self, v: &Coord) {
        self.descriptor.cframe.position += *v;
    }

    fn rotate(&mut self, v: &Direction) {
        self.descriptor.cframe.orientation += *v;
    }

    fn get_cframe(&self) -> &CFrame {
        &self.descriptor.cframe
    }

    fn parse(&mut self, _node: &serde_json::Value) {}
    fn load_obj(&mut self, _path: &str) {}
    fn reflect_ray(&self, _ray: &mut Ray, _face: &Face) {}
    fn compute_sdf(&self, _point: &Coord) -> (f32, &Face) {
        (f32::MAX, &Vec::new())
    }
    fn compute_hit(&self, _point: &Coord, _face: Option<&Face>) -> Coord {
        Coord::new(0.0, 0.0, 0.0)
    }
    fn set_immunity(&mut self, object: Option<Box<dyn Object>>) {
        self.immunity = object;
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
    }

    fn reset(&mut self) {
        for ray in self.rays.iter_mut() {
            ray.reset();
            // TODO: and everything else
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
