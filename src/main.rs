use colored::*;
use std::{env, fs::File, io::Write, path::PathBuf};

use crate::{
    config::loader::PluginLoader,
    raytracer::{Raytracer, camera::Camera, structs::Color},
};

mod config;
mod ffi;
mod raytracer;
mod utils;

pub type Error = Box<dyn std::error::Error + Send + Sync + 'static>;

fn print_help() {
    println!("{}", "PROJECT".bold());
    println!(
        "\t{} {} {}",
        "This project aim to make a 'full' raytracer".cyan(),
        "the".cyan(),
        "'raytracer'".yellow()
    );
    println!(
        "{}",
        "\tYou can make your own plugins and use them with our documentation".cyan()
    );
    println!(
        "{}",
        "\tIt should be able to run on the raytracer if the documentation is respected!!!".cyan()
    );
    println!();

    println!("{}", "USAGE".bold());
    let usage_color = |s: &str| s.magenta();
    println!(
        "\t{}",
        usage_color(
            "./raytracer <scene_cfg_path> [-a]  [-c <used_camera_path>] [-p <plugins_directory_path>] [-o <obj_directory_path>] [-s <ppm_directory_path>] [-r \"wxh\"]"
        )
    );
    println!(
        "\t{}",
        usage_color(
            "./raytracer <scene_cfg_path> -gui [-c <used_camera_path>] [-p <plugins_directory_path>] [-o <obj_directory_path>] [-r \"wxh\"]"
        )
    );
    println!("\t{}", usage_color("./raytracer <ppm_file_path>"));
    println!("\t{}", usage_color("./raytracer -h"));
    println!();

    println!("{}", "INFORMATION".bold());
    println!(
        "\t{}\tSwitch to ppm viewer mode and display the given ppm file",
        "./raytracer <ppm_file_path>".magenta(),
    );
    println!();

    println!("{}", "OPTIONS".bold());

    let opt = |name: &str, desc: &str| {
        println!("\t{}", name.green());
        println!("\t\t{}", desc);
    };

    opt("-h, --help", "Write the informations of the executable");
    opt("-gui", "Activate the render at runtime");
    opt(
        "-a, --advencement",
        "Enable the advencement display of the actual frame rendering",
    );

    println!("\t{} ({})", "-n, --newton".green(), "Not Working".red());
    println!("\t\tActivate the newton mode, apply aproximative gravity on rays");

    println!(
        "\t{} <{}>",
        "-c, --camera".green(),
        "used_camera_path".red()
    );
    println!("\t\tForce the camera plugin used (default: first found)");

    println!(
        "\t{} <{}>",
        "-p, --plugins".green(),
        "plugins_directory_path".red()
    );
    println!(
        "\t\tSet the plugins path, can also be set by the env var RAYTRACER_PLUGINS_PATH (default: \"{}\")",
        "./plugins/".red()
    );

    println!("\t{} <{}>", "-o, --obj".green(), "obj_directory_path".red());

    println!(
        "\t\tSet the obj path, can also be set by the env var RAYTRACER_OBJ_PATH (default: \"{}\")",
        "./obj/".red()
    );

    println!(
        "\t{} <{}>",
        "-s, --save".green(),
        "ppm_directory_path".red()
    );

    println!(
        "\t\tSet the ppm save path, can also be set by the env var RAYTRACER_RENDERED_PATH (default: \"{}\")",
        "./rendered/".red()
    );

    println!(
        "\t{} \"{}x{}\"",
        "-r, --resolution".green(),
        "w".red(),
        "h".red()
    );
    println!("\t\tForce the camera resolution used (default: set in the cfg)");
    println!();

    println!("{}", "INPUT (only -gui)".bold());
    let input = |key: &str, action: &str| {
        println!("\t{}\t{}", key.green(), action);
    };

    input("ESC", "Leave the rendering");
    input("q", "Translate camera left");
    input("d", "Translate camera right");
    input("z", "Translate camera forward");
    input("s", "Translate camera backward");
    input("e", "Translate camera up");
    input("a", "Translate camera down");
    input("←", "Rotate camera left");
    input("→", "Rotate camera right");
    input("↑", "Rotate camera up");
    input("↓", "Rotate camera down");
}

pub fn print_progress_bar(msg: &str, current: usize, total: usize) {
    let width: usize = 40;
    let progress: usize = (current * width) / total;

    let bar_done = "█".repeat(progress);
    let bar_remaining = "░".repeat(width - progress);

    let current_colored = if current == total {
        current.to_string().green()
    } else {
        current.to_string().yellow()
    };

    let total_colored = if current == total {
        total.to_string().green()
    } else {
        total.to_string().red()
    };

    print!(
        "\r{} {}{} ({}/{})",
        msg.bright_black(),
        bar_done.green(),
        bar_remaining.to_string().red(),
        current_colored,
        total_colored,
    );
    std::io::stdout().flush().ok();
}

fn save_as_ppm(
    filename: &PathBuf,
    pixels: &[Color],
    width: u32,
    height: u32,
) -> std::io::Result<()> {
    let mut file = File::create(filename)?;
    write!(&mut file, "P3\n{width} {height}\n255\n")?;

    let total_pixels = pixels.len();

    let update_interval = width as usize;

    for (i, p) in pixels.iter().enumerate() {
        writeln!(&mut file, "{} {} {}", p.x, p.y, p.z)?;
        if i % update_interval == 0 {
            print_progress_bar("Saving:", i, total_pixels);
        }
    }

    print_progress_bar("Saving:", total_pixels, total_pixels);
    println!();
    Ok(())
}

fn run(raytracer: &mut Raytracer, args: Vec<String>) -> Result<(), Error> {
    raytracer.parse_flags(args);
    let filepath = if raytracer.settings.viewer {
        raytracer.settings.ppm_path.clone()
    } else {
        "scene.json".to_string()
    };

    println!(
        "{} '{}'{}",
        "Loading scene from".bright_black(),
        filepath.underline(),
        "...".bright_black(),
    );

    let mut loader = if raytracer.settings.plugins_set
        && let Some(path) = raytracer.settings.plugins_path.to_str()
    {
        PluginLoader::new(path.to_string())
    } else {
        PluginLoader::new("./plugins".to_string())
    };
    let scene = config::load_scene(&filepath, &mut loader)?;

    raytracer.camera = scene.camera;
    raytracer.objects = scene.objects;
    raytracer.lights = scene.lights;
    raytracer.plugins = Some(loader);

    if raytracer.settings.resolution_set {
        raytracer.camera.resolution = raytracer.settings.resolution;
    }

    let ppm_path = if raytracer.settings.rendered_set {
        raytracer.settings.rendered_path.clone()
    } else {
        "output.ppm".into()
    };

    if raytracer.settings.gui {
        raytracer.gui()?;
    } else {
        raytracer.render_once(raytracer.settings.adv);
        println!();

        raytracer.camera.update_screen();

        println!(
            "{} '{}'...",
            "Generating image to".bright_black(),
            ppm_path.to_string_lossy().underline()
        );
        save_as_ppm(
            &ppm_path,
            raytracer.camera.get_screen(),
            raytracer.camera.resolution.0,
            raytracer.camera.resolution.1,
        )
        .expect("Error saving image");

        println!(
            "{} '{}'",
            "Successfully generated".green(),
            ppm_path.to_string_lossy().underline()
        );
    }
    Ok(())
}

fn main() {
    let mut raytracer = Raytracer::default();
    let args: Vec<String> = env::args().collect();

    for arg in &args {
        if arg == "-h" || arg == "--help" {
            print_help();
            return;
        }
    }

    if let Err(e) = run(&mut raytracer, args) {
        eprintln!("{e}");
    }
}
