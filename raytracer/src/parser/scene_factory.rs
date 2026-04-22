use crate::core::color::Color;
use crate::lights::directional::DirectionalLight;
use crate::lights::light::Light;
use crate::math::vec3::Vec3;
use crate::parser::config_types::{DirectionalLightConfig, PlaneConfig, SphereConfig};
use crate::primitives::plane::Plane;
use crate::primitives::primitive::Primitive;
use crate::primitives::sphere::Sphere;

pub struct SceneFactory;

impl SceneFactory {
    pub fn vec3_from_array(values: [f64; 3]) -> Vec3 {
        Vec3::new(values[0], values[1], values[2])
    }

    pub fn color_from_array(values: [f64; 3]) -> Color {
        Color::new(values[0], values[1], values[2])
    }

    pub fn create_sphere(cfg: SphereConfig) -> Box<dyn Primitive> {
        Box::new(Sphere::new(
            Self::vec3_from_array(cfg.center),
            cfg.radius,
            Self::color_from_array(cfg.color),
        ))
    }

    pub fn create_plane(cfg: PlaneConfig) -> Box<dyn Primitive> {
        Box::new(Plane::new(
            Self::vec3_from_array(cfg.point),
            Self::vec3_from_array(cfg.normal),
            Self::color_from_array(cfg.color),
        ))
    }

    pub fn create_directional_light(cfg: DirectionalLightConfig) -> Box<dyn Light> {
        Box::new(DirectionalLight::new(
            Self::vec3_from_array(cfg.direction),
            cfg.intensity,
        ))
    }
}
