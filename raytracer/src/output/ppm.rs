use std::fs::File;
use std::io::{BufWriter, Write};

use crate::core::color::Color;

pub fn write_ppm(
    path: &str,
    width: usize,
    height: usize,
    pixels: &[Color],
) -> std::io::Result<()> {
    let file = File::create(path)?;
    let mut writer = BufWriter::new(file);

    writeln!(writer, "P3")?;
    writeln!(writer, "{} {}", width, height)?;
    writeln!(writer, "255")?;

    for color in pixels {
        let (r, g, b) = color.to_rgb8();
        writeln!(writer, "{} {} {}", r, g, b)?;
    }

    Ok(())
}
