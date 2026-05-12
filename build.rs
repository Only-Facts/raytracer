use cmake::Config;

fn main() {
    Config::new(".")
        .define("CMAKE_BUILD_TYPE", "Release")
        .build_target("all")
        .build();
}
