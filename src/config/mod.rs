use crate::{
    config::loader::PluginLoader,
    ffi::bridge::{LightBridge, MaterialBridge, ObjectBridge},
    raytracer::{camera::Viewer, structs::Coord},
    utils::vector::Vector3,
};
use colored::Colorize;
use libloading::Library;
use serde_json::Value;
use std::sync::Arc;

pub mod loader;

pub struct Scene {
    pub camera: Viewer,
    pub objects: Vec<ObjectBridge>,
    pub lights: Vec<LightBridge>,
    _libraries: Vec<Arc<Library>>,
}

pub fn load_scene(filepath: &str, loader: &mut PluginLoader) -> Result<Scene, String> {
    let data = std::fs::read_to_string(filepath).map_err(|e| {
        format!(
            "{} reading file {}: {e}",
            "Error".red(),
            filepath.underline(),
        )
    })?;

    let root: Value = serde_json::from_str(&data).map_err(|e| format!("JSON Syntax error: {e}"))?;

    let cam_val = root
        .get("camera")
        .ok_or("No section 'camera' in JSON file")?;

    let res_x = cam_val
        .get("resolution_x")
        .and_then(|v| v.as_u64())
        .unwrap_or(800) as u32;
    let res_y = cam_val
        .get("resolution_y")
        .and_then(|v| v.as_u64())
        .unwrap_or(600) as u32;
    let fov = cam_val.get("fov").and_then(|v| v.as_f64()).unwrap_or(90.0);

    let pos_val = cam_val.get("position");
    let cam_pos = Coord::new(
        pos_val
            .and_then(|p| p.get("x"))
            .and_then(|v| v.as_f64())
            .unwrap_or(0.0),
        pos_val
            .and_then(|p| p.get("y"))
            .and_then(|v| v.as_f64())
            .unwrap_or(0.0),
        pos_val
            .and_then(|p| p.get("z"))
            .and_then(|v| v.as_f64())
            .unwrap_or(-5.0),
    );

    let dir_val = cam_val.get("rotation");

    let rx = dir_val
        .and_then(|p| p.get("x"))
        .and_then(|v| v.as_f64())
        .unwrap_or(0.0_f64)
        .to_radians();
    let ry = dir_val
        .and_then(|p| p.get("y"))
        .and_then(|v| v.as_f64())
        .unwrap_or(0.0_f64)
        .to_radians();
    let cam_dir = Coord::new(ry.sin() * rx.cos(), -rx.sin(), ry.cos() * rx.cos());

    let camera = Viewer::new(cam_pos, cam_dir, res_x, res_y, fov);

    let mut objects = Vec::new();
    let mut lights = Vec::new();
    let mut libraries = Vec::new();

    if let Some(primitives) = root.get("primitives").and_then(|p| p.as_array()) {
        for prim in primitives {
            let obj_type = prim
                .get("type")
                .and_then(|t| t.as_str())
                .unwrap_or("sphere");

            let mat_path = "./plugins/materials/material_default.so";
            let mat_lib = match loader.get_library(mat_path) {
                Ok(l) => l,
                Err(e) => {
                    println!(
                        "{}: Failed to load {}, ignoring... ({})",
                        "Warning".yellow(),
                        mat_path.underline(),
                        e.red()
                    );
                    continue;
                }
            };
            let material = MaterialBridge::new(mat_lib)?;

            let lib_path = format!("./plugins/objects/object_{obj_type}.so");

            let lib = match loader.get_library(&lib_path) {
                Ok(l) => l,
                Err(e) => {
                    println!(
                        "{}: Failed to load {}, ignoring... ({})",
                        "Warning".yellow(),
                        lib_path.underline(),
                        e.red()
                    );
                    continue;
                }
            };
            libraries.push(Arc::clone(&lib));

            match ObjectBridge::new(lib, obj_type.to_string()) {
                Ok(mut bridge) => {
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

                    if let Some(radius) = prim.get("radius").and_then(|v| v.as_f64()) {
                        bridge.set_radius(radius);
                    }

                    if let Some(color) = prim.get("color") {
                        let r = color.get("r").and_then(|v| v.as_u64()).unwrap_or(255) as u8;
                        let g = color.get("g").and_then(|v| v.as_u64()).unwrap_or(255) as u8;
                        let b = color.get("b").and_then(|v| v.as_u64()).unwrap_or(255) as u8;
                        bridge.set_material(material.instance);
                        bridge.set_color(r, g, b);
                    } else {
                        bridge.set_color(255, 255, 255);
                    }

                    objects.push(bridge);
                }
                Err(e) => {
                    println!(
                        "{}: Error instantiating object {}: {}",
                        "Warning".yellow(),
                        obj_type.underline(),
                        e.red()
                    );
                }
            }
        }
    } else {
        return Err(format!(
            "{}: failed to find '{}' table in JSON file",
            "Error".red(),
            "primitives".underline()
        ));
    }

    if let Some(light_list) = root.get("lights").and_then(|l| l.as_array()) {
        for l_conf in light_list {
            let l_type = l_conf
                .get("type")
                .and_then(|t| t.as_str())
                .unwrap_or("point");
            let lib_path = format!("./plugins/lights/light_{l_type}.so");

            match loader.get_library(&lib_path) {
                Ok(lib) => {
                    libraries.push(Arc::clone(&lib));
                    if let Ok(bridge) = LightBridge::new(lib) {
                        if let Some(pos) = l_conf.get("position") {
                            let x = pos.get("x").and_then(|v| v.as_f64()).unwrap_or(0.0);
                            let y = pos.get("y").and_then(|v| v.as_f64()).unwrap_or(0.0);
                            let z = pos.get("z").and_then(|v| v.as_f64()).unwrap_or(0.0);
                            bridge.set_position(Vector3::new(x, y, z));
                        }

                        if let Some(intensity) = l_conf.get("intensity").and_then(|v| v.as_f64()) {
                            bridge.set_intensity(intensity);
                        }

                        if let Some(color_arr) = l_conf.get("color").and_then(|c| c.as_array())
                            && color_arr.len() >= 3
                        {
                            let r = color_arr[0].as_u64().unwrap_or(255) as u8;
                            let g = color_arr[1].as_u64().unwrap_or(255) as u8;
                            let b = color_arr[2].as_u64().unwrap_or(255) as u8;
                            bridge.set_color(r, g, b);
                        }

                        lights.push(bridge);
                    }
                }
                Err(e) => {
                    println!(
                        "{}: Failed to load {}, ignoring... ({})",
                        "Warning".yellow(),
                        lib_path.underline(),
                        e.red()
                    );
                }
            }
        }
    } else {
        return Err(format!(
            "{}: failed to find '{}' table in JSON file",
            "Error".red(),
            "lights".underline()
        ));
    }

    Ok(Scene {
        camera,
        objects,
        lights,
        _libraries: libraries,
    })
}
