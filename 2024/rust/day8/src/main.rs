#![allow(dead_code)]

use std::cmp::PartialEq;
use std::collections::HashSet;
use std::env;
use std::fmt;
use std::fs;

#[derive(Hash, Clone)]
struct Coord {
    row: usize,
    col: usize,
}

impl PartialEq for Coord {
    fn eq(&self, other: &Self) -> bool {
        self.row == other.row && self.col == other.col
    }
}

impl Eq for Coord {}

struct Offset(i16, i16);

struct Vec2d<T> {
    vec: Vec<T>,
    rows: usize,
    cols: usize,
}

impl<T> IntoIterator for Vec2d<T> {
    type Item = T;
    type IntoIter = std::vec::IntoIter<Self::Item>;

    fn into_iter(self) -> Self::IntoIter {
        self.vec.into_iter()
    }
}

impl Coord {
    fn from_1d_index(_rows: usize, cols: usize, index: usize) -> Self {
        Coord {
            row: index / cols,
            col: index % cols,
        }
    }
    fn to_1d_index(&self, _rows: usize, cols: usize) -> usize {
        self.col + cols * self.row
    }
    fn from_option(row: Option<usize>, col: Option<usize>) -> Option<Coord> {
        let r: usize = match row {
            Some(x) => x,
            _ => return None,
        };
        let c: usize = match col {
            Some(x) => x,
            _ => return None,
        };
        Some(Coord { row: r, col: c })
    }
    fn within_bounds(&self, rows: usize, cols: usize) -> bool {
        self.row < rows && self.col < cols
    }
    fn offset(&self, offset: &Offset) -> Option<Self> {
        let r_o: usize = if offset.0 < 0 {
            offset.0.abs() as usize
        } else {
            offset.0 as usize
        };
        let c_o: usize = if offset.1 < 0 {
            offset.1.abs() as usize
        } else {
            offset.1 as usize
        };

        Coord::from_option(
            if offset.0 < 0 {
                self.row.checked_sub(r_o)
            } else {
                Some(self.row + r_o)
            },
            if offset.1 < 0 {
                self.col.checked_sub(c_o)
            } else {
                Some(self.col + c_o)
            },
        )
    }
}

impl fmt::Display for Coord {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "({}, {})", self.row, self.col)
    }
}

impl<T: PartialEq> Vec2d<T> {
    fn new(vec: Vec<T>, rows: usize, cols: usize) -> Self {
        Vec2d { vec, rows, cols }
    }

    fn get_by_coord(&self, coord: &Coord) -> &T {
        &self.vec[coord.to_1d_index(self.rows, self.cols)]
    }
}

impl fmt::Display for Vec2d<char> {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        let mut str_res: String = String::with_capacity(self.vec.len() + (self.rows - 1));
        for i in 0..self.rows {
            let start: usize = i * self.rows;
            let end: usize = start + self.rows;
            let mut str_line: String = self.vec[start..end].into_iter().collect();
            if i != self.rows - 1 {
                str_line.push('\n');
            }
            str_res.push_str(&str_line);
        }
        write!(f, "{}", str_res)
    }
}

fn antinodes(map: &Vec2d<char>) -> u32 {
    let mut nodes: HashSet<Coord> = HashSet::new();

    let mut count: u32 = 0;

    for ref_index in 0..map.vec.len() {
        let ref_coord: Coord = Coord::from_1d_index(map.rows, map.cols, ref_index);
        let ref_c: char = *map.get_by_coord(&ref_coord);

        if ref_c == '.' {
            continue;
        }

        for iter_index in (ref_index + 1)..map.vec.len() {
            let iter_coord: Coord = Coord::from_1d_index(map.rows, map.cols, iter_index);
            let iter_c: char = *map.get_by_coord(&iter_coord);

            if iter_c == ref_c {
                match Coord::from_option(
                    (2 * ref_coord.row).checked_sub(iter_coord.row),
                    (2 * ref_coord.col).checked_sub(iter_coord.col),
                ) {
                    Some(coord) => {
                        nodes.insert(coord);
                    }
                    _ => (),
                };

                match Coord::from_option(
                    (2 * iter_coord.row).checked_sub(ref_coord.row),
                    (2 * iter_coord.col).checked_sub(ref_coord.col),
                ) {
                    Some(coord) => {
                        nodes.insert(coord);
                    }
                    _ => (),
                };
            }
        }
    }

    for node in nodes.iter() {
        if node.within_bounds(map.rows, map.cols) {
            count = count + 1;
        }
    }

    count
}

fn antirays(map: &Vec2d<char>) -> u32 {
    let mut nodes: HashSet<Coord> = HashSet::new();

    for ref_index in 0..map.vec.len() {
        let ref_coord: Coord = Coord::from_1d_index(map.rows, map.cols, ref_index);
        let ref_c: char = *map.get_by_coord(&ref_coord);

        if ref_c == '.' {
            continue;
        }

        for iter_index in 0..map.vec.len() {
            if iter_index == ref_index {
                continue;
            }

            let iter_coord: Coord = Coord::from_1d_index(map.rows, map.cols, iter_index);
            let iter_c: char = *map.get_by_coord(&iter_coord);

            if iter_c == ref_c {
                let offset: Offset = Offset(
                    iter_coord.row as i16 - ref_coord.row as i16,
                    iter_coord.col as i16 - ref_coord.col as i16,
                );

                let mut antinode: Coord = iter_coord.clone();

                loop {
                    if !antinode.within_bounds(map.rows, map.cols) {
                        break;
                    }
                    nodes.insert(antinode.clone());
                    antinode = match antinode.offset(&offset) {
                        Some(n) => n,
                        _ => {
                            break;
                        }
                    };
                }
            }
        }
    }

    nodes.len() as u32
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let file_contents = fs::read_to_string(&args[1]).expect("Could not read file");
    let mut lines: Vec<&str> = file_contents.split("\n").collect();
    if lines[lines.len() - 1].bytes().len() == 0 {
        lines.remove(lines.len() - 1);
    }
    let mut map_flat: Vec<char> = Vec::new();
    for line in &lines {
        map_flat.extend(line.chars().collect::<Vec<char>>());
    }
    let map: Vec2d<char> = Vec2d::new(map_flat, lines.len(), lines[0].len());
    println!("{:?}", antinodes(&map));
    println!("{:?}", antirays(&map));
}
