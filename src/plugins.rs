use std::{
    collections::HashMap,
    ffi::{CStr, c_void},
    path::PathBuf,
    sync::Arc,
};

use libloading::{Library, Symbol};
use tokio::sync::RwLock;

use crate::ffi::{PluginFactoryFunc, PluginNameFunc, PluginTypeFunc};

pub struct PluginWrapper {
    _lib: Arc<Library>,
    pub plugin_type: usize,
    pub name: String,
    factory_ptr: PluginFactoryFunc,
}

impl PluginWrapper {
    pub fn create_instance(&self) -> *mut c_void {
        unsafe { (self.factory_ptr)() }
    }
}

#[derive(Default)]
pub struct PluginManager {
    plugins: RwLock<HashMap<String, Arc<PluginWrapper>>>,
}

impl PluginManager {
    pub async fn load_plugin(&self, path: PathBuf) -> Result<(), String> {
        let wrapper = tokio::task::spawn_blocking(move || -> Result<PluginWrapper, String> {
            unsafe {
                let lib =
                    Library::new(path.as_os_str()).map_err(|e| format!("dlopen error: {e}"))?;

                let type_func: Symbol<PluginTypeFunc> = lib
                    .get(b"type\0")
                    .map_err(|_| "Symbol 'type' not found".to_string())?;
                let name_func: Symbol<PluginNameFunc> = lib
                    .get(b"name\0")
                    .map_err(|_| "Symbol 'name' not found".to_string())?;
                let factory_func: Symbol<PluginFactoryFunc> = lib
                    .get(b"factory\0")
                    .map_err(|_| "Symbol 'factory' not found".to_string())?;

                let plugin_type = type_func();
                let c_name = name_func();
                let name = CStr::from_ptr(c_name).to_string_lossy().into_owned();

                let factory_ptr = *factory_func;

                Ok(PluginWrapper {
                    _lib: Arc::new(lib),
                    plugin_type,
                    name,
                    factory_ptr,
                })
            }
        })
        .await
        .map_err(|e| format!("thread error: {e}"))??;

        let mut write_lock = self.plugins.write().await;

        let key = format!("{}{}", wrapper.name.to_lowercase(), wrapper.plugin_type);
        write_lock.insert(key, Arc::new(wrapper));

        Ok(())
    }
}

