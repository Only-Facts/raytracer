use std::sync::{
    Arc, Mutex,
    atomic::{AtomicBool, Ordering},
};

use sfml::{
    graphics::{Color as SfColor, PrimitiveType, RenderStates, RenderTarget, RenderWindow, Vertex},
    system::Vector2f,
    window::{Event, Key},
};

use crate::raytracer::{camera, structs::Color};

pub type SharedPixels = Arc<Mutex<Vec<Color>>>;
pub type SharedInput = Arc<Mutex<InputState>>;

#[derive(Debug, Clone, Copy, Default)]
pub struct InputState {
    translate_x: f64,
    translate_y: f64,
    translate_z: f64,

    rotate_yaw: f64,
    rotate_pitch: f64,
}

pub fn loop_window_threaded(
    window: &mut RenderWindow,
    shared_pixels: SharedPixels,
    shared_input: SharedInput,
    running: Arc<AtomicBool>,
    width: usize,
    height: usize,
) {
    while window.is_open() {
        match window.poll_event() {
            Some(
                Event::Closed
                | Event::KeyPressed {
                    code: Key::Escape, ..
                },
            ) => {
                window.close();
                return;
            }
            Some(_) => {}
            None => {}
        }

        {
            let mut input = shared_input.lock().expect("shared input mutex poisoned");

            *input = read_input_state();
        }

        let pixels_snapshot = {
            let pixels = shared_pixels
                .lock()
                .expect("shared pixel buffer mutex poisoned");

            pixels.clone()
        };

        draw_pixels(window, &pixels_snapshot, width, height);
    }

    running.store(false, Ordering::Relaxed);
}

fn draw_pixels(window: &mut RenderWindow, pixels: &[Color], width: usize, height: usize) {
    let mut vertices = Vec::with_capacity(width * height);

    for y in 0..height {
        for x in 0..width {
            let index = y * width + x;

            let Some(color) = pixels.get(index) else {
                continue;
            };

            let position = Vector2f::new(x as f32, y as f32);
            let sfml_color = SfColor::rgb(color.x, color.y, color.z);

            vertices.push(Vertex::with_pos_color(position, sfml_color));
        }
    }

    window.clear(SfColor::BLACK);

    window.draw_primitives(&vertices, PrimitiveType::POINTS, &RenderStates::default());

    window.display();
}

fn read_input_state() -> InputState {
    let mut input = InputState::default();

    if Key::D.is_pressed() {
        input.translate_x -= 1.0;
    }

    if Key::Q.is_pressed() {
        input.translate_x += 1.0;
    }

    if Key::Z.is_pressed() {
        input.translate_z += 1.0;
    }

    if Key::S.is_pressed() {
        input.translate_z -= 1.0;
    }

    if Key::E.is_pressed() {
        input.translate_y += 1.0;
    }

    if Key::A.is_pressed() {
        input.translate_y -= 1.0;
    }

    if Key::Left.is_pressed() {
        input.rotate_yaw += 1.0;
    }

    if Key::Right.is_pressed() {
        input.rotate_yaw -= 1.0;
    }

    if Key::Up.is_pressed() {
        input.rotate_pitch -= 1.0;
    }

    if Key::Down.is_pressed() {
        input.rotate_pitch += 1.0;
    }

    input
}

pub fn apply_input(camera: &mut camera::Viewer, input: InputState) {
    let translation_speed = 0.5;
    let rotation_speed = 0.03;

    let has_translation =
        input.translate_x != 0.0 || input.translate_y != 0.0 || input.translate_z != 0.0;

    if has_translation {
        camera.translate_local(
            input.translate_x * translation_speed,
            input.translate_y * translation_speed,
            input.translate_z * translation_speed,
        );
    }

    let has_rotation = input.rotate_yaw != 0.0 || input.rotate_pitch != 0.0;

    if has_rotation {
        camera.rotate(
            input.rotate_yaw * rotation_speed,
            input.rotate_pitch * rotation_speed,
        );
    }
}
