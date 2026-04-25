use config::{Config, File};

use crate::parser::config_types::SceneConfig;

pub fn load_scene_config(path: &str) -> Result<SceneConfig, String> {
    let settings = Config::builder()
        .add_source(File::with_name(path))
        .build()
        .map_err(|e| format!("Failed to read config file: {e}"))?;

    settings
        .try_deserialize::<SceneConfig>()
        .map_err(|e| format!("Failed to parse scene file: {e}"))
}
