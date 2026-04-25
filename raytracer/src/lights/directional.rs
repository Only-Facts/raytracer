use crate::core::hit::HitRecord;
use crate::lights::light::Light;
use crate::math::vec3::Vec3;

pub struct DirectionalLight {
    pub direction: Vec3,
    pub intensity: f64,
}

impl DirectionalLight {
    pub fn new(direction: Vec3, intensity: f64) -> Self {
        Self {
            direction: direction.normalize(),
            intensity,
        }
    }
}

impl Light for DirectionalLight {
    fn compute(&self, hit: &HitRecord) -> f64 {
        hit.normal.dot(-self.direction).max(0.0) * self.intensity
    }
}
