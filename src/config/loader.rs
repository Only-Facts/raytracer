use serde::Deserialize;

use crate::{raytracer::structs::Coord, utils::vector::Vector3};

#[derive(Deserialize)]
pub struct ObjectConfig {
    pub r#type: String,
    pub position: [f64; 3],
    pub color: [u8; 3],
}

#[derive(Deserialize)]
pub struct CameraConfig {
    pub resolution_x: u32,
    pub resolution_y: u32,
    pub fov: f64,
    pub position: Vector3<Coord>,
    pub rotation: Vector3<Coord>,
}

#[derive(Deserialize)]
pub struct LightConfig {
    pub r#type: String,
    pub position: Vector3<Coord>,
}

#[derive(Deserialize)]
pub struct SceneConfig {
    pub camera: CameraConfig,
    pub objects: Vec<ObjectConfig>,
    pub lights: Vec<LightConfig>,
}
