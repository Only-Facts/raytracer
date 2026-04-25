use crate::core::color::Color;
use crate::core::hit::HitRecord;
use crate::math::ray::Ray;
use crate::math::vec3::Vec3;
use crate::primitives::primitive::Primitive;

pub struct Plane {
    pub point: Vec3,
    pub normal: Vec3,
    pub color: Color,
}

impl Plane {
    pub fn new(point: Vec3, normal: Vec3, color: Color) -> Self {
        Self {
            point,
            normal: normal.normalize(),
            color,
        }
    }
}

impl Primitive for Plane {
    fn intersect(&self, ray: &Ray) -> Option<HitRecord> {
        let denom = self.normal.dot(ray.direction);

        if denom.abs() < 1e-6 {
            return None;
        }

        let t = (self.point - ray.origin).dot(self.normal) / denom;

        if t <= 0.0 {
            return None;
        }

        let point = ray.at(t);

        Some(HitRecord {
            t,
            point,
            normal: self.normal,
            color: self.color,
        })
    }
}
