use crate::core::camera::Camera;
use crate::lights::light::Light;
use crate::primitives::primitive::Primitive;

pub struct Scene {
    pub camera: Camera,
    pub primitives: Vec<Box<dyn Primitive>>,
    pub lights: Vec<Box<dyn Light>>,
    pub ambient_intensity: f64,
}
