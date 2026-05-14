use crate::{ffi::bridge::ObjectBridge, utils::vector::Vector3};
use libloading::Library;
use serde_json::Value;
use std::sync::Arc;

pub mod loader;

pub struct Scene {
    pub objects: Vec<ObjectBridge>,
    _libraries: Vec<Arc<Library>>,
}

pub fn load_scene(filepath: &str) -> Result<Scene, String> {
    let data = std::fs::read_to_string(filepath)
        .map_err(|e| format!("Error reading file {filepath}: {e}"))?;

    let root: Value = serde_json::from_str(&data).map_err(|e| format!("JSON Syntax error: {e}"))?;

    let mut objects = Vec::new();
    let mut libraries = Vec::new();

    if let Some(primitives) = root.get("primitives").and_then(|p| p.as_array()) {
        for prim in primitives {
            let obj_type = prim
                .get("type")
                .and_then(|t| t.as_str())
                .unwrap_or("sphere");

            let lib_path = format!("./plugins/objects/object_{obj_type}.so");

            let lib = match unsafe { Library::new(&lib_path) } {
                Ok(l) => Arc::new(l),
                Err(e) => {
                    println!("Warning: Failed to load {lib_path}, ignoring... ({e})");
                    continue;
                }
            };
            libraries.push(lib.clone());

            match ObjectBridge::new(lib) {
                Ok(bridge) => {
                    if let Some(pos) = prim.get("position") {
                        let x = pos.get("x").and_then(|v| v.as_f64()).unwrap_or(0.0);
                        let y = pos.get("y").and_then(|v| v.as_f64()).unwrap_or(0.0);
                        let z = pos.get("z").and_then(|v| v.as_f64()).unwrap_or(0.0);
                        bridge.set_position(Vector3::new(x, y, z));
                    }

                    if let Some(rot) = prim.get("rotation") {
                        let x = rot.get("x").and_then(|v| v.as_f64()).unwrap_or(0.0);
                        let y = rot.get("y").and_then(|v| v.as_f64()).unwrap_or(0.0);
                        let z = rot.get("z").and_then(|v| v.as_f64()).unwrap_or(0.0);
                        bridge.set_orientation(Vector3::new(x, y, z));
                    }

                    objects.push(bridge);
                }
                Err(e) => {
                    println!("Warning: Error instantiating object {obj_type}: {e}");
                }
            }
        }
    } else {
        return Err("Error: failed to find 'primitives' table in JSON file".to_string());
    }

    Ok(Scene {
        objects,
        _libraries: libraries,
    })
}
