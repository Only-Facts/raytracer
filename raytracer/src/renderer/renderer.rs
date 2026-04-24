use crate::core::color::Color;
use crate::core::hit::HitRecord;
use crate::core::scene::Scene;
use crate::math::ray::Ray;

pub struct Renderer {
    pub background: Color,
}

impl Renderer {
    pub fn new(background: Color) -> Self {
        Self { background }
    }

    pub fn render(&self, scene: &Scene) -> Vec<Color> {
        let width = scene.camera.width;
        let height = scene.camera.height;
        let mut pixels = Vec::with_capacity(width * height);

        for y in 0..height {
            for x in 0..width {
                let ray = scene.camera.generate_ray(x, y);
                let color = self.trace_ray(&ray, scene);
                pixels.push(color);
            }
        }

        pixels
    }

    fn trace_ray(&self, ray: &Ray, scene: &Scene) -> Color {
        match self.closest_hit(ray, scene) {
            Some(hit) => self.shade(&hit, scene),
            None => self.background,
        }
    }

    fn closest_hit(&self, ray: &Ray, scene: &Scene) -> Option<HitRecord> {
        let mut closest: Option<HitRecord> = None;

        for primitive in &scene.primitives {
            if let Some(hit) = primitive.intersect(ray) {
                if hit.t > 0.0 && closest.as_ref().map_or(true, |current| hit.t < current.t) {
                    closest = Some(hit);
                }
            }
        }

        closest
    }

    fn shade(&self, hit: &HitRecord, scene: &Scene) -> Color {
        let ambient = hit.color * scene.ambient_intensity;

        let mut diffuse = Color::new(0.0, 0.0, 0.0);
        for light in &scene.lights {
            let factor = light.compute(hit);
            diffuse = diffuse + (hit.color * factor);
        }

        (ambient + diffuse).clamp()
    }
}
