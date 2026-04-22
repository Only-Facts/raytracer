use crate::core::camera::Camera;
use crate::core::scene::Scene;
use crate::lights::light::Light;
use crate::primitives::primitive::Primitive;

pub struct SceneBuilder {
    camera: Option<Camera>,
    primitives: Vec<Box<dyn Primitive>>,
    lights: Vec<Box<dyn Light>>,
    ambient_intensity: Option<f64>,
}

impl SceneBuilder {
    pub fn new() -> Self {
        Self {
            camera: None,
            primitives: Vec::new(),
            lights: Vec::new(),
            ambient_intensity: None,
        }
    }

    pub fn with_camera(mut self, camera: Camera) -> Self {
        self.camera = Some(camera);
        self
    }

    pub fn add_primitive(mut self, primitive: Box<dyn Primitive>) -> Self {
        self.primitives.push(primitive);
        self
    }

    pub fn add_light(mut self, light: Box<dyn Light>) -> Self {
        self.lights.push(light);
        self
    }

    pub fn with_ambient(mut self, ambient: f64) -> Self {
        self.ambient_intensity = Some(ambient);
        self
    }

    pub fn build(self) -> Result<Scene, String> {
        Ok(Scene {
            camera: self.camera.ok_or("Missing camera")?,
            primitives: self.primitives,
            lights: self.lights,
            ambient_intensity: self.ambient_intensity.unwrap_or(0.0),
        })
    }
}
