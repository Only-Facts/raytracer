mod math;
mod core;
mod primitives;
mod lights;
mod renderer;
mod output;
mod parser;
mod cli;

use crate::cli::parse_args;
use crate::core::camera::Camera;
use crate::core::color::Color;
use crate::output::ppm::write_ppm;
use crate::parser::loader::load_scene_config;
use crate::parser::scene_builder::SceneBuilder;
use crate::parser::scene_factory::SceneFactory;
use crate::renderer::renderer::Renderer;

fn run() -> Result<(), String> {
    let args = parse_args()?;
    let scene_cfg = load_scene_config(&args.scene_file)?;

    let camera = Camera::new(
        scene_cfg.camera.width,
        scene_cfg.camera.height,
        scene_cfg.camera.fov,
        SceneFactory::vec3_from_array(scene_cfg.camera.position),
    );

    let mut builder = SceneBuilder::new()
        .with_camera(camera)
        .with_ambient(scene_cfg.lights.ambient)
        .add_light(SceneFactory::create_directional_light(
            scene_cfg.lights.directional,
        ));

    for sphere_cfg in scene_cfg.primitives.spheres {
        builder = builder.add_primitive(SceneFactory::create_sphere(sphere_cfg));
    }

    builder = builder.add_primitive(SceneFactory::create_plane(scene_cfg.primitives.plane));

    let scene = builder.build()?;

    let renderer = Renderer::new(Color::new(20.0, 20.0, 30.0));
    let pixels = renderer.render(&scene);

    write_ppm("image.ppm", scene.camera.width, scene.camera.height, &pixels)
        .map_err(|e| format!("Failed to write output image: {e}"))?;

    println!("Rendered image.ppm");
    Ok(())
}

fn main() {
    if let Err(err) = run() {
        eprintln!("{err}");
        std::process::exit(84);
    }
}
