#[repr(C)]
#[derive(Debug, Clone, Copy)]
#[allow(dead_code)]
pub struct CVector3 {
    pub x: f64,
    pub y: f64,
    pub z: f64,
}

impl From<crate::utils::vector::Vector3<f64>> for CVector3 {
    fn from(v: crate::utils::vector::Vector3<f64>) -> Self {
        Self {
            x: v.x,
            y: v.y,
            z: v.z,
        }
    }
}
