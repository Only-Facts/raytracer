use serde::Deserialize;

use crate::ffi::bridge::ObjectBridge;

#[derive(Deserialize)]
pub struct SceneConfig {
    pub camera: CameraConfig,
    pub objects: Vec<ObjectBridge>,
    pub lights: Vec<LightConfig>,
}

#[derive(Deserialize)]
pub struct ObjectConfig {
    pub r#type: String,
    pub position: [f64; 3],
    pub color: [u8; 3],
}
