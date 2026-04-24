use crate::core::hit::HitRecord;

pub trait Light: Send + Sync {
    fn compute(&self, hit: &HitRecord) -> f64;
}
