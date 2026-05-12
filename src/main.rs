use colored::*;

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

    // --- USAGE ---
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

fn main() {
    print_help();
}
