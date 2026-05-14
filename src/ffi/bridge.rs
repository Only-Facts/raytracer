use std::{ffi::c_void, sync::Arc};

use libloading::{Library, Symbol};

use crate::{raytracer::structs::Color, utils::vector::Vector3};

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
                set_position_ptr: *lib.get(b"object_set_position\0").unwrap(),
                set_orientation_ptr: *lib.get(b"object_set_orientation\0").unwrap(),
                compute_sdf_ptr: *lib.get(b"object_compute_sdf\0").unwrap(),
                compute_hit_ptr: *lib.get(b"object_compute_hit\0").unwrap(),
                get_point_color_ptr: *lib.get(b"object_get_point_color\0").unwrap(),
                is_mirror_ptr: *lib.get(b"object_is_mirror\0").unwrap(),
                destroy_ptr: *lib.get(b"destroy_instance\0").unwrap(),
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
    get_pixels_ptr: FnGetPointColor,
}

type FnLightGetPos = unsafe extern "C" fn(*mut c_void, *mut f64, *mut f64, *mut f64);
type FnLightGetColor = unsafe extern "C" fn(*mut c_void, *mut u8, *mut u8, *mut u8, *mut f64);
type FnLightSetPos = unsafe extern "C" fn(*mut c_void, f64, f64, f64);
type FnLightIsGlobal = unsafe extern "C" fn(*mut c_void, *mut bool);


pub struct LightBridge {
    _lib: Arc<Library>,
    pub instance: *mut c_void,
    get_pos_ptr: FnLightGetPos,
    get_color_ptr: FnLightGetColor,
    set_pos_ptr: FnLightSetPos,
    is_global_ptr: FnLightIsGlobal,
    destroy_ptr: FnDestroy,
}

unsafe impl Send for LightBridge {}
unsafe impl Sync for LightBridge {}

impl LightBridge {
    pub fn new(lib: Arc<Library>) -> Result<Self, String> {
        unsafe {
            let factory_ptr: Symbol<FnFactory> = lib
                .get(b"factory\0")
                .map_err(|e| format!("factory error: {e}"))?;
            let instance = factory_ptr();

            Ok(Self {
                _lib: lib.clone(),
                instance,
                get_pos_ptr: *lib.get(b"light_get_position\0").unwrap(),
                get_color_ptr: *lib.get(b"light_get_color\0").unwrap(),
                set_pos_ptr: *lib.get(b"light_set_position\0").unwrap(),
                is_global_ptr: *lib.get(b"light_is_global\0").unwrap(),
                destroy_ptr: *lib.get(b"destroy_instance\0").unwrap(),
            })
        }
    }

    pub fn get_position(&self) -> Vector3<f64> {
        let (mut x, mut y, mut z) = (0.0, 0.0, 0.0);
        unsafe { (self.get_pos_ptr)(self.instance, &mut x, &mut y, &mut z) };
        Vector3::new(x, y, z)
    }

    pub fn get_color_info(&self) -> (Color, f64) {
        let (mut r, mut g, mut b) = (0, 0, 0);
        let mut intensity = 0.0;
        unsafe { (self.get_color_ptr)(self.instance, &mut r, &mut g, &mut b, &mut intensity) };
        (Color::new(r, g, b), intensity)
    }

    pub fn set_position(&self, pos: Vector3<f64>) {
        unsafe { (self.set_pos_ptr)(self.instance, pos.x, pos.y, pos.z) };
    }

    pub fn is_global(&self) -> bool {
        let mut global = false;
        unsafe { (self.is_global_ptr)(self.instance, &mut global) };
        global
    }
}

impl Drop for LightBridge {
    fn drop(&mut self) {
        unsafe { (self.destroy_ptr)(self.instance) }
    }
}
