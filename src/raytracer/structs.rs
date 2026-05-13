use crate::utils::vector::Vector3;

const SPACE_CHUNK_SIZE: i64 = 50;

pub type Type = f64;
pub type Coord = Vector3<Type>;
pub type Direction = Vector3<Type>;
pub type Color = Vector3<u8>;

pub type Chunk = Vector3<i64>;

pub type Vertice = Vector3<Type>;
pub type Face = Vec<Vertice>;

#[derive(Default, Clone)]
pub struct CFrame {
    pub position: Coord,
    pub orientation: Direction,
    pub rotation: Type,
}

pub fn get_space_chunk(point: &Coord, space_chunk_size: i64) -> Chunk {
    Chunk {
        x: point.x as i64 / space_chunk_size,
        y: point.y as i64 / space_chunk_size,
        z: point.z as i64 / space_chunk_size,
    }
}
