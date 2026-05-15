use cmake::Config;

fn main() {
    println!("cargo::rerun-if-changed=src-cpp/");
    println!("cargo::rerun-if-changed=include/");
    println!("cargo::rerun-if-changed=CMakeLists.txt");
    println!("cargo::rerun-if-changed=build.rs");

    Config::new(".")
        .define("CMAKE_BUILD_TYPE", "Optimized")
        .build_target("all")
        .build();
}
