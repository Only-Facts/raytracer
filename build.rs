use cmake::Config;

fn main() {
    cargo_build::warning("Rebuilding CMake");
    cargo_build::rerun_if_changed(["src-cpp", "include", "CMakeLists.txt", "build.rs"]);

    Config::new(".")
        .define("CMAKE_BUILD_TYPE", "Released")
        .build_target("plugins")
        .build();
}
