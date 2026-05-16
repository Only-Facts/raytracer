use colored::*;
use libloading::Library;
use serde::Deserialize;
use std::{collections::HashMap, path::PathBuf, sync::Arc};

use crate::{raytracer::structs::Coord, utils::vector::Vector3};

#[derive(Deserialize)]
#[allow(dead_code)]
pub struct ObjectConfig {
    pub r#type: String,
    pub position: [f64; 3],
    pub color: [u8; 3],
}

#[derive(Deserialize)]
#[allow(dead_code)]
pub struct CameraConfig {
    pub resolution_x: u32,
    pub resolution_y: u32,
    pub fov: f64,
    pub position: Vector3<Coord>,
    pub rotation: Vector3<Coord>,
}

#[derive(Deserialize)]
#[allow(dead_code)]
pub struct LightConfig {
    pub r#type: String,
    pub position: Vector3<Coord>,
}

#[derive(Deserialize)]
#[allow(dead_code)]
pub struct SceneConfig {
    pub camera: CameraConfig,
    pub objects: Vec<ObjectConfig>,
    pub lights: Vec<LightConfig>,
}

#[derive(Default, Clone)]
pub struct PluginLoader {
    cache: HashMap<String, Arc<Library>>,
    pub plugins_path: String,
}

impl PluginLoader {
    pub fn new(plugins_path: String) -> Self {
        Self {
            cache: HashMap::new(),
            plugins_path,
        }
    }
    pub fn get_library(&mut self, path: &str) -> Result<Arc<Library>, String> {
        if let Some(lib) = self.cache.get(path) {
            return Ok(Arc::clone(lib));
        }

        unsafe {
            let lib =
                Library::new(path).map_err(|e| format!("Failed to load library {path}: {e}"))?;
            let shared_lib = Arc::new(lib);
            self.cache.insert(path.to_string(), Arc::clone(&shared_lib));
            println!(
                "{} {}",
                "Successfully loaded".green(),
                path.underline().bright_black()
            );
            Ok(shared_lib)
        }
    }
}
