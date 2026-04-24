use crate::core::hit::HitRecord;
use crate::math::ray::Ray;

pub trait Primitive: Send + Sync {
    fn intersect(&self, ray: &Ray) -> Option<HitRecord>;
}
