use std::collections::{HashMap, HashSet};

use crate::raytracer::{
    objects::Object,
    structs::{CFrame, Chunk, Color, Type, get_space_chunk},
};

#[derive(Debug, Clone)]
pub struct RayBase {
    pub cframe: CFrame,
    pub alive: bool,
    pub distance: Type,
    pub potential_objects: Vec<usize>,
}

impl RayBase {
    pub fn new(cframe: CFrame) -> Self {
        Self {
            cframe,
            alive: true,
            distance: 0.0,
            potential_objects: Vec::new(),
        }
    }

    pub fn kill(&mut self) {
        self.alive = false;
    }

    #[allow(dead_code)]
    pub fn compute_objects(
        &mut self,
        render_distance: Type,
        all_objects: &[Box<dyn Object>],
        chunks_map: &HashMap<Chunk, Vec<usize>>,
        space_chunk_size: Type,
    ) {
        let render_dist_sq = render_distance * render_distance;
        let pos_origin = self.cframe.position;
        let mut current_pos = self.cframe.position;
        let direction = self.cframe.orientation;

        let step = direction * (space_chunk_size / direction.norm());

        let mut seen = HashSet::new();
        self.potential_objects.clear();

        for (id, obj_box) in all_objects.iter().enumerate() {
            if obj_box.get_descriptor().faces.is_empty() {
                self.potential_objects.push(id);
                seen.insert(id);
            }
        }

        while (current_pos - pos_origin).norm_squared() < render_dist_sq {
            let chunk = get_space_chunk(&current_pos, space_chunk_size as i64);

            if let Some(objects_indices) = chunks_map.get(&chunk) {
                for &id in objects_indices {
                    if !seen.contains(&id) {
                        let obj = &all_objects[id];

                        if obj.will_collide(&current_pos, &direction) {
                            self.potential_objects.push(id);
                            seen.insert(id);
                        }
                    }
                }
            }
            current_pos += step;

            if seen.len() == all_objects.len() {
                break;
            }
        }
    }
}

#[derive(Debug, Clone)]
pub struct Ray {
    pub base: RayBase,
    pub color: Color,
    pub coef: f32,
}

impl Ray {
    pub fn reset(&mut self) {
        self.base.alive = true;
        self.base.distance = 0.0;
        self.color = Color::new(0, 0, 0);
        self.coef = 1.0;
    }

    pub fn set_color(&mut self, color: Color) {
        self.color = color;
    }
}

#[allow(dead_code)]
pub struct LightRay {
    pub base: RayBase,
    pub color: Color,
    pub intensity: f32,
    pub lumen: f32,
}

#[allow(dead_code)]
impl LightRay {
    pub fn get_luminescence(&self, unit_meter_coef: Type) -> f32 {
        if self.lumen == -1.0 {
            self.intensity
        } else {
            let d = self.base.distance / unit_meter_coef;
            self.intensity * self.lumen / (4.0 * std::f32::consts::PI * (d * d) as f32)
        }
    }

    pub fn clone_ray(&self) -> Self {
        Self {
            base: RayBase {
                cframe: CFrame { ..self.base.cframe },
                alive: true,
                distance: self.base.distance,
                potential_objects: self.base.potential_objects.clone(),
            },
            color: self.color,
            intensity: self.intensity,
            lumen: self.lumen,
        }
    }
}
