use crate::core::color::Color;
use crate::math::vec3::Vec3;

#[derive(Clone, Copy, Debug)]
pub struct HitRecord {
    pub t: f64,
    pub point: Vec3,
    pub normal: Vec3,
    pub color: Color,
}
