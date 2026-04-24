pub struct CliArgs {
    pub scene_file: String,
}

pub fn parse_args() -> Result<CliArgs, String> {
    let args: Vec<String> = std::env::args().collect();

    match args.len() {
        2 => {
            if args[1] == "--help" {
                print_help();
                std::process::exit(0);
            }

            Ok(CliArgs {
                scene_file: args[1].clone(),
            })
        }
        _ => Err(String::from("Invalid arguments. Use --help for usage.")),
    }
}

pub fn print_help() {
    println!("USAGE: ./raytracer <SCENE_FILE>");
    println!("  SCENE_FILE: scene configuration");
}
