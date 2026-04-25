use crate::math::ray::Ray;
use crate::math::vec3::Vec3;

pub struct Camera {
    pub width: usize,
    pub height: usize,
    pub fov_degrees: f64,
    pub position: Vec3,
}

impl Camera {
    pub fn new(width: usize, height: usize, fov_degrees: f64, position: Vec3) -> Self {
        Self {
            width,
            height,
            fov_degrees,
            position,
        }
    }

    pub fn generate_ray(&self, x: usize, y: usize) -> Ray {
        let aspect_ratio = self.width as f64 / self.height as f64;
        let fov_adjustment = (self.fov_degrees.to_radians() / 2.0).tan();

        let sensor_x = (((x as f64 + 0.5) / self.width as f64) * 2.0 - 1.0) * aspect_ratio * fov_adjustment;
        let sensor_y = (1.0 - ((y as f64 + 0.5) / self.height as f64) * 2.0) * fov_adjustment;

        let direction = Vec3::new(sensor_x, sensor_y, -1.0).normalize();
        Ray::new(self.position, direction)
    }
}
