use std::ffi::{c_char, c_void};

pub mod bridge;
pub mod types;

pub const PLUGIN_TYPE_CAMERA: usize = 0;
pub const PLUGIN_TYPE_LIGHT: usize = 1;
pub const PLUGIN_TYPE_OBJECT: usize = 2;
pub const PLUGIN_TYPE_MATERIAL: usize = 3;

pub type PluginTypeFunc = unsafe extern "C" fn() -> usize;
pub type PluginNameFunc = unsafe extern "C" fn() -> *const c_char;
pub type PluginFactoryFunc = unsafe extern "C" fn() -> *mut c_void;
