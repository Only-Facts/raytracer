use serde::Deserialize;

#[derive(Debug, Deserialize)]
pub struct SceneConfig {
    pub camera: CameraConfig,
    pub lights: LightsConfig,
    pub primitives: PrimitivesConfig,
}

#[derive(Debug, Deserialize)]
pub struct CameraConfig {
    pub width: usize,
    pub height: usize,
    pub position: [f64; 3],
    pub fov: f64,
}

#[derive(Debug, Deserialize)]
pub struct LightsConfig {
    pub ambient: f64,
    pub directional: DirectionalLightConfig,
}

#[derive(Debug, Deserialize)]
pub struct DirectionalLightConfig {
    pub direction: [f64; 3],
    pub intensity: f64,
}

#[derive(Debug, Deserialize)]
pub struct PrimitivesConfig {
    pub spheres: Vec<SphereConfig>,
    pub plane: PlaneConfig,
}

#[derive(Debug, Deserialize)]
pub struct SphereConfig {
    pub center: [f64; 3],
    pub radius: f64,
    pub color: [f64; 3],
}

#[derive(Debug, Deserialize)]
pub struct PlaneConfig {
    pub point: [f64; 3],
    pub normal: [f64; 3],
    pub color: [f64; 3],
}
