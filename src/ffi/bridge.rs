use std::{ffi::c_void, sync::Arc};

use libloading::Library;

#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub struct CSdfResult {
    pub distance: f64,
    pub face_ptr: *const c_void,
}

type FnComputeSdf = unsafe extern "C" fn(*mut c_void, f64, f64, f64) -> CSdfResult;
type FnDestroy = unsafe extern "C" fn(*mut c_void);

pub struct ObjectBridge {
    _lib: Arc<Library>,
    instance: *mut c_void,
    compute_sdf_ptr: FnComputeSdf,
    destroy_ptr: FnDestroy,
}

impl ObjectBridge {
    pub fn new(lib: Arc<Library>, instance: *mut c_void) -> Result<Self, String> {
        unsafe {
            let compute_sdf_sym: libloading::Symbol<FnComputeSdf> = lib
                .get(b"object_compute_sdf\0")
                .map_err(|e| format!("Symbol 'object_compute_sdf' not found: {e}"))?;
            let destroy_sym: libloading::Symbol<FnDestroy> = lib
                .get(b"destroy_instance\0")
                .map_err(|e| format!("Symbol 'destroy_instance' not found: {e}"))?;

            Ok(Self {
                _lib: lib,
                instance,
                compute_sdf_ptr: *compute_sdf_sym,
                destroy_ptr: *destroy_sym,
            })
        }
    }

    #[inline(always)]
    pub fn compute_sdf(&self, point: crate::utils::vector::Vector3<f64>) -> CSdfResult {
        unsafe { (self.compute_sdf_ptr)(self.instance, point.x, point.y, point.z) }
    }
}

impl Drop for ObjectBridge {
    fn drop(&mut self) {
        unsafe {
            (self.destroy_ptr)(self.instance);
        }
    }
}
