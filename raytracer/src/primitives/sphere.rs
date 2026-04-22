use crate::core::color::Color;
use crate::core::hit::HitRecord;
use crate::math::ray::Ray;
use crate::math::vec3::Vec3;
use crate::primitives::primitive::Primitive;

pub struct Sphere {
    pub center: Vec3,
    pub radius: f64,
    pub color: Color,
}

impl Sphere {
    pub fn new(center: Vec3, radius: f64, color: Color) -> Self {
        Self { center, radius, color }
    }
}

impl Primitive for Sphere {
    fn intersect(&self, ray: &Ray) -> Option<HitRecord> {
        let oc = ray.origin - self.center;

        let a = ray.direction.dot(ray.direction);
        let b = 2.0 * oc.dot(ray.direction);
        let c = oc.dot(oc) - self.radius * self.radius;

        let discriminant = b * b - 4.0 * a * c;

        if discriminant < 0.0 {
            return None;
        }

        let sqrt_d = discriminant.sqrt();

        let mut t = (-b - sqrt_d) / (2.0 * a);
        if t <= 0.0 {
            t = (-b + sqrt_d) / (2.0 * a);
            if t <= 0.0 {
                return None;
            }
        }

        let point = ray.at(t);
        let normal = (point - self.center).normalize();

        Some(HitRecord {
            t,
            point,
            normal,
            color: self.color,
        })
    }
}
