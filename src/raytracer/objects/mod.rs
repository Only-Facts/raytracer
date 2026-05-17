use crate::raytracer::{
    ray::Ray,
    structs::{CFrame, Chunk, Color, Coord, Direction, Face},
};

pub mod sphere;

#[allow(dead_code)]
#[derive(Default, Debug, Clone)]
pub struct ObjectDescriptor {
    pub cframe: CFrame,
    pub cframe_origin: CFrame,

    pub chunks: Vec<Chunk>,
    pub faces: Vec<Face>,
}

#[allow(dead_code)]
pub struct ChunkLightData {
    pub position: Coord,
    pub color: Color,
    pub intensity: f32,
}

#[allow(dead_code)]
pub trait Object: Send {
    fn get_descriptor(&self) -> &ObjectDescriptor;
    fn will_collide(&self, point: &Coord, dir: &Direction) -> bool;

    fn parse(&mut self, node: &serde_json::Value);
    fn load_obj(&mut self, path: &str);

    fn reflect_ray(&self, ray: &mut Ray, face: &Face);
    fn compute_sdf(&self, point: &Coord) -> (f32, &Face);
    fn compute_hit(&self, point: &Coord, face: Option<&Face>) -> Coord;

    fn translate(&mut self, v: &Coord);
    fn rotate(&mut self, v: &Direction);

    fn get_cframe(&self) -> &CFrame;
}
