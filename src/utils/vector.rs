use std::ops::{Add, AddAssign, Div, DivAssign, Mul, MulAssign, Sub, SubAssign};

pub trait Real: Sized + Copy + Add<Output = Self> + Mul<Output = Self> {
    fn sqrt(self) -> Self;
}

macro_rules! impl_real_float {
    ($($t:ty),*) => {
        $(
            impl Real for $t {
                fn sqrt(self) -> Self {
                    self.sqrt()
                }
            }
        )*
    };
}

impl_real_float!(f32, f64);

macro_rules! impl_real_int {
    ($($t:ty),*) => {
        $(
            impl Real for $t {
                fn sqrt(self) -> Self {
                    (self as f64).sqrt() as $t
                }
            }
        )*
    };
}

impl_real_int!(
    i8, i16, i32, i64, i128, isize, u8, u16, u32, u64, u128, usize
);

#[derive(Default, Clone, Copy, Debug, PartialEq, Eq, Hash)]
pub struct Vector2<T> {
    pub x: T,
    pub y: T,
}

impl<T> Vector2<T> {
    pub fn new(x: T, y: T) -> Self {
        Self { x, y }
    }
}

impl<T: Real> Vector2<T> {
    pub fn norm_squared(&self) -> T {
        self.x * self.x + self.y * self.y
    }

    pub fn norm(&self) -> T {
        self.norm_squared().sqrt()
    }

    pub fn length(&self) -> T {
        self.norm()
    }
}

impl<T: Add<Output = T>> Add for Vector2<T> {
    type Output = Self;
    fn add(self, rhs: Self) -> Self {
        Self::new(self.x + rhs.x, self.y + rhs.y)
    }
}

impl<T: Sub<Output = T>> Sub for Vector2<T> {
    type Output = Self;
    fn sub(self, rhs: Self) -> Self {
        Self::new(self.x - rhs.x, self.y - rhs.y)
    }
}

impl<T: AddAssign> AddAssign for Vector2<T> {
    fn add_assign(&mut self, rhs: Self) {
        self.x += rhs.x;
        self.y += rhs.y;
    }
}

impl<T: SubAssign> SubAssign for Vector2<T> {
    fn sub_assign(&mut self, rhs: Self) {
        self.x -= rhs.x;
        self.y -= rhs.y;
    }
}

macro_rules! impl_vector_scalar_ops {
    ($($t:ty),*) => {
        $(
            impl Add<$t> for Vector2<$t> {
                type Output = Self;
                fn add(self, rhs: $t) -> Self { Self::new(self.x + rhs, self.y + rhs) }
            }
            impl Sub<$t> for Vector2<$t> {
                type Output = Self;
                fn sub(self, rhs: $t) -> Self { Self::new(self.x - rhs, self.y - rhs) }
            }
            impl Mul<$t> for Vector2<$t> {
                type Output = Self;
                fn mul(self, rhs: $t) -> Self { Self::new(self.x * rhs, self.y * rhs) }
            }
            impl Div<$t> for Vector2<$t> {
                type Output = Self;
                fn div(self, rhs: $t) -> Self { Self::new(self.x / rhs, self.y / rhs) }
            }

            impl Add<Vector2<$t>> for $t {
                type Output = Vector2<$t>;
                fn add(self, rhs: Vector2<$t>) -> Vector2<$t> { Vector2::new(self + rhs.x, self + rhs.y) }
            }
            impl Sub<Vector2<$t>> for $t {
                type Output = Vector2<$t>;
                fn sub(self, rhs: Vector2<$t>) -> Vector2<$t> { Vector2::new(self - rhs.x, self - rhs.y) }
            }
            impl Mul<Vector2<$t>> for $t {
                type Output = Vector2<$t>;
                fn mul(self, rhs: Vector2<$t>) -> Vector2<$t> { Vector2::new(self * rhs.x, self * rhs.y) }
            }
            impl Div<Vector2<$t>> for $t {
                type Output = Vector2<$t>;
                fn div(self, rhs: Vector2<$t>) -> Vector2<$t> { Vector2::new(self / rhs.x, self / rhs.y) }
            }

            impl AddAssign<$t> for Vector2<$t> {
                fn add_assign(&mut self, rhs: $t) {
                    self.x += rhs; self.y += rhs;
                }
            }
            impl SubAssign<$t> for Vector2<$t> {
                fn sub_assign(&mut self, rhs: $t) {
                    self.x -= rhs; self.y -= rhs;
                }
            }
            impl MulAssign<$t> for Vector2<$t> {
                fn mul_assign(&mut self, rhs: $t) {
                    self.x *= rhs; self.y *= rhs;
                }
            }
            impl DivAssign<$t> for Vector2<$t> {
                fn div_assign(&mut self, rhs: $t) {
                    self.x /= rhs; self.y /= rhs;
                }
            }
        )*
    };
}

impl_vector_scalar_ops!(
    f32, f64, i8, i16, i32, i64, i128, isize, u8, u16, u32, u64, u128, usize
);

#[derive(Default, Clone, Copy, Debug, PartialEq, Eq, Hash)]
pub struct Vector3<T> {
    pub x: T,
    pub y: T,
    pub z: T,
}

impl<T> Vector3<T> {
    pub fn new(x: T, y: T, z: T) -> Self {
        Self { x, y, z }
    }
}

impl<T: Real> Vector3<T> {
    pub fn norm_squared(&self) -> T {
        self.x * self.x + self.y * self.y + self.z * self.z
    }

    pub fn norm(&self) -> T {
        self.norm_squared().sqrt()
    }

    pub fn length(&self) -> T {
        self.norm()
    }
}

impl<T: Add<Output = T>> Add for Vector3<T> {
    type Output = Self;
    fn add(self, rhs: Self) -> Self {
        Self::new(self.x + rhs.x, self.y + rhs.y, self.z + rhs.z)
    }
}

impl<T: Sub<Output = T>> Sub for Vector3<T> {
    type Output = Self;
    fn sub(self, rhs: Self) -> Self {
        Self::new(self.x - rhs.x, self.y - rhs.y, self.z - rhs.z)
    }
}

impl<T: AddAssign> AddAssign for Vector3<T> {
    fn add_assign(&mut self, rhs: Self) {
        self.x += rhs.x;
        self.y += rhs.y;
        self.z += rhs.z;
    }
}

impl<T: SubAssign> SubAssign for Vector3<T> {
    fn sub_assign(&mut self, rhs: Self) {
        self.x -= rhs.x;
        self.y -= rhs.y;
        self.z -= rhs.z;
    }
}

macro_rules! impl_vector_scalar_ops {
    ($($t:ty),*) => {
        $(
            impl Add<$t> for Vector3<$t> {
                type Output = Self;
                fn add(self, rhs: $t) -> Self { Self::new(self.x + rhs, self.y + rhs, self.z + rhs) }
            }
            impl Sub<$t> for Vector3<$t> {
                type Output = Self;
                fn sub(self, rhs: $t) -> Self { Self::new(self.x - rhs, self.y - rhs, self.z - rhs) }
            }
            impl Mul<$t> for Vector3<$t> {
                type Output = Self;
                fn mul(self, rhs: $t) -> Self { Self::new(self.x * rhs, self.y * rhs, self.z * rhs) }
            }
            impl Div<$t> for Vector3<$t> {
                type Output = Self;
                fn div(self, rhs: $t) -> Self { Self::new(self.x / rhs, self.y / rhs, self.z / rhs) }
            }

            impl Add<Vector3<$t>> for $t {
                type Output = Vector3<$t>;
                fn add(self, rhs: Vector3<$t>) -> Vector3<$t> { Vector3::new(self + rhs.x, self + rhs.y, self + rhs.z) }
            }
            impl Sub<Vector3<$t>> for $t {
                type Output = Vector3<$t>;
                fn sub(self, rhs: Vector3<$t>) -> Vector3<$t> { Vector3::new(self - rhs.x, self - rhs.y, self - rhs.z) }
            }
            impl Mul<Vector3<$t>> for $t {
                type Output = Vector3<$t>;
                fn mul(self, rhs: Vector3<$t>) -> Vector3<$t> { Vector3::new(self * rhs.x, self * rhs.y, self * rhs.z) }
            }
            impl Div<Vector3<$t>> for $t {
                type Output = Vector3<$t>;
                fn div(self, rhs: Vector3<$t>) -> Vector3<$t> { Vector3::new(self / rhs.x, self / rhs.y, self / rhs.z) }
            }

            impl AddAssign<$t> for Vector3<$t> {
                fn add_assign(&mut self, rhs: $t) {
                    self.x += rhs; self.y += rhs; self.z += rhs;
                }
            }
            impl SubAssign<$t> for Vector3<$t> {
                fn sub_assign(&mut self, rhs: $t) {
                    self.x -= rhs; self.y -= rhs; self.z -= rhs;
                }
            }
            impl MulAssign<$t> for Vector3<$t> {
                fn mul_assign(&mut self, rhs: $t) {
                    self.x *= rhs; self.y *= rhs; self.z *= rhs;
                }
            }
            impl DivAssign<$t> for Vector3<$t> {
                fn div_assign(&mut self, rhs: $t) {
                    self.x /= rhs; self.y /= rhs; self.z /= rhs;
                }
            }
        )*
    };
}

impl_vector_scalar_ops!(
    f32, f64, i8, i16, i32, i64, i128, isize, u8, u16, u32, u64, u128, usize
);
