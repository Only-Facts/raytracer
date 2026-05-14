use std::{ffi::c_void, sync::Arc};

use libloading::{Library, Symbol};

use crate::utils::vector::Vector3;

#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub struct CSdfResult {
    pub distance: f64,
    pub face_ptr: *const c_void,
}

#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub struct CHitResult {
    pub nx: f64,
    pub ny: f64,
    pub nz: f64,
}

#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub struct CColorResult {
    pub r: u8,
    pub g: u8,
    pub b: u8,
    pub ok: bool,
}

type FnSetPosition = unsafe extern "C" fn(*mut c_void, f64, f64, f64);
type FnSetOrientation = unsafe extern "C" fn(*mut c_void, f64, f64, f64);
type FnComputeSdf = unsafe extern "C" fn(*mut c_void, f64, f64, f64) -> CSdfResult;
type FnComputeHit = unsafe extern "C" fn(*mut c_void, f64, f64, f64, *const c_void) -> CHitResult;
type FnGetPointColor = unsafe extern "C" fn(*mut c_void, f64, f64, f64) -> CColorResult;
type FnIsMirror = unsafe extern "C" fn(*mut c_void) -> bool;
type FnDestroy = unsafe extern "C" fn(*mut c_void);
type FnFactory = unsafe extern "C" fn() -> *mut c_void;

pub struct ObjectBridge {
    _lib: Arc<Library>,
    pub instance: *mut c_void,

    set_position_ptr: FnSetPosition,
    set_orientation_ptr: FnSetOrientation,
    compute_sdf_ptr: FnComputeSdf,
    compute_hit_ptr: FnComputeHit,
    get_point_color_ptr: FnGetPointColor,
    is_mirror_ptr: FnIsMirror,
    destroy_ptr: FnDestroy,
}

unsafe impl Send for ObjectBridge {}
unsafe impl Sync for ObjectBridge {}

impl ObjectBridge {
    pub fn new(lib: Arc<Library>) -> Result<Self, String> {
        unsafe {
            let factory_ptr: Symbol<FnFactory> = lib
                .get(b"factory\0")
                .map_err(|e| format!("factory error: {e}"))?;
            let instance = factory_ptr();

            Ok(Self {
                _lib: lib.clone(),
                instance,
                set_position_ptr: *lib.get(b"object_set_position\0").unwrap_or_default(),
                set_orientation_ptr: *lib.get(b"object_set_orientation\0").unwrap_or_default(),
                compute_sdf_ptr: *lib.get(b"object_compute_sdf\0").unwrap_or_default(),
                compute_hit_ptr: *lib.get(b"object_compute_hit\0").unwrap_or_default(),
                get_point_color_ptr: *lib.get(b"object_get_point_color\0").unwrap_or_default(),
                is_mirror_ptr: *lib.get(b"object_is_mirror\0").unwrap_or_default(),
                destroy_ptr: *lib.get(b"destroy_instance\0").unwrap_or_default(),
            })
        }
    }

    #[inline(always)]
    pub fn set_position(&self, pos: Vector3<f64>) {
        unsafe { (self.set_position_ptr)(self.instance, pos.x, pos.y, pos.z) }
    }

    #[inline(always)]
    pub fn set_orientation(&self, rot: Vector3<f64>) {
        unsafe { (self.set_orientation_ptr)(self.instance, rot.x, rot.y, rot.z) }
    }

    #[inline(always)]
    pub fn compute_sdf(&self, point: Vector3<f64>) -> CSdfResult {
        unsafe { (self.compute_sdf_ptr)(self.instance, point.x, point.y, point.z) }
    }

    #[inline(always)]
    pub fn compute_hit(&self, point: Vector3<f64>, face: *const c_void) -> Vector3<f64> {
        unsafe {
            let res = (self.compute_hit_ptr)(self.instance, point.x, point.y, point.z, face);
            Vector3::new(res.nx, res.ny, res.nz)
        }
    }

    #[inline(always)]
    pub fn get_point_color(&self, point: Vector3<f64>) -> CColorResult {
        unsafe { (self.get_point_color_ptr)(self.instance, point.x, point.y, point.z) }
    }

    #[inline(always)]
    pub fn is_mirror(&self) -> bool {
        unsafe { (self.is_mirror_ptr)(self.instance) }
    }
}

impl Drop for ObjectBridge {
    fn drop(&mut self) {
        unsafe {
            (self.destroy_ptr)(self.instance);
        }
    }
}

pub struct CameraBridge {
    instance: *mut c_void,
    get_pixels_ptr: FnGetPixel,
}

impl CameraBridge {
    pub fn get_pixels(&self, width: usize, height: usize) -> &[CColor] {
        unsafe {
            let ptr = (self.get_pixels_ptr)(self.instance);
            std::slice::from_raw_parts(ptr as *const CColor, width * height)
        }
    }
}
