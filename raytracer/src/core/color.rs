use std::ops::{Add, Mul};

#[derive(Clone, Copy, Debug)]
pub struct Color {
    pub r: f64,
    pub g: f64,
    pub b: f64,
}

impl Color {
    pub fn new(r: f64, g: f64, b: f64) -> Self {
        Self { r, g, b }
    }

    pub fn clamp(self) -> Self {
        Self {
            r: self.r.clamp(0.0, 255.0),
            g: self.g.clamp(0.0, 255.0),
            b: self.b.clamp(0.0, 255.0),
        }
    }

    pub fn to_rgb8(self) -> (u8, u8, u8) {
        let c = self.clamp();
        (c.r as u8, c.g as u8, c.b as u8)
    }
}

impl Add for Color {
    type Output = Self;

    fn add(self, rhs: Self) -> Self::Output {
        Self::new(self.r + rhs.r, self.g + rhs.g, self.b + rhs.b)
    }
}

impl Mul<f64> for Color {
    type Output = Self;

    fn mul(self, rhs: f64) -> Self::Output {
        Self::new(self.r * rhs, self.g * rhs, self.b * rhs)
    }
}
