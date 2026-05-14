use cmake::Config;

fn main() {
    println!("cargo::rerun-if-changed=src-cpp/");
    println!("cargo::rerun-if-changed=CMakeLists.txt");
    println!("cargo::rerun-if-changed=build.rs");

    Config::new(".")
        .define("CMAKE_BUILD_TYPE", "Release")
        .build_target("all")
        .build();
}
