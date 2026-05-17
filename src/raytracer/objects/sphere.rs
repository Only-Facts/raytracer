use crate::raytracer::structs::{Color, Coord};

#[derive(Default, Debug, Clone, Copy)]
#[allow(unused)]
pub struct Sphere {
    pub position: Coord,
    pub radius: f64,
    pub color: Color,
}
