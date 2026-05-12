use cmake::Config;
use std::env;

fn main() {
    Config::new(".")
        .define("CMAKE_BUILD_TYPE", "Release")
        .build_target("all")
        .build();

    let out_dir = env::var("OUT_DIR").unwrap();
    println!("Plugins compiled in : {out_dir}/build/plugins");
}
