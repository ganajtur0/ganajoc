use std::env;
use std::fmt;
use std::fs;

#[derive(Debug)]
pub enum Directions {
    N,
    E,
    S,
    W,
}

#[derive(Debug, Copy, Clone)]
struct Coords(usize, usize);

#[derive(Debug)]
struct Vec2d<T> {
    vec: Vec<T>,
    rows: usize,
    cols: usize,
}

impl<T: std::cmp::PartialEq> Vec2d<T> {
    fn new(vec: Vec<T>, rows: usize, cols: usize) -> Self {
        assert!(vec.len() == rows * cols);
        Self { vec, rows, cols }
    }

    fn row(&self, row: usize) -> &[T] {
        let i = self.cols * row;
        &self.vec[i..(i + self.cols)]
    }

    fn index_by_coords(&self, coords: &Coords) -> &T {
        let i = self.cols * coords.0;
        &self.vec[i + coords.1]
    }

    fn set_by_coord(&mut self, value: T, coords: &Coords) {
        let i = self.cols * coords.0;
        self.vec[i + coords.1] = value;
    }

    fn find(&self, element: &T) -> Option<Coords> {
        let pos = self.vec.iter().position(|x| x == element);
        match pos {
            Some(x) => Some(Coords(x / self.rows, x % self.rows)),
            _ => None,
        }
    }

    fn within_bounds(&self, coords: &Coords) -> bool {
        coords.0 != usize::MAX
            && coords.1 != usize::MAX
            && coords.0 < self.rows
            && coords.1 < self.cols
    }
}

impl<T: std::fmt::Debug + std::cmp::PartialEq> std::fmt::Display for Vec2d<T> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let mut str = String::new();
        for i in 0..self.rows {
            if i != 0 {
                str.push('\n');
            }
            str.push_str(&format!("{:?}", &self.row(i)));
        }
        write!(f, "{}", str)
    }
}

fn walk_around(mut map: Vec2d<char>) -> u32 {
    let mut steps: u32 = 1;
    let mut coords: Coords = map.find(&'^').unwrap();
    let mut direction: Directions = Directions::N;

    map.set_by_coord('X', &coords);

    loop {
        let mut coords_tmp = coords;

        match direction {
            Directions::N => coords_tmp.0 -= 1,
            Directions::S => coords_tmp.0 += 1,
            Directions::E => coords_tmp.1 += 1,
            Directions::W => coords_tmp.1 -= 1,
        }

        if !map.within_bounds(&coords_tmp) {
            break;
        }

        if *map.index_by_coords(&coords_tmp) == '#' {
            direction = match direction {
                Directions::N => Directions::E,
                Directions::S => Directions::W,
                Directions::E => Directions::S,
                Directions::W => Directions::N,
            }
        } else {
            coords = coords_tmp;
            steps += if *map.index_by_coords(&coords) == 'X' {
                0
            } else {
                1
            };
            map.set_by_coord('X', &coords);
        }
    }
    steps
}

fn main() {
    let args: Vec<String> = env::args().collect();

    let file_contents = fs::read_to_string(&args[1]).expect("Could not read the file.");

    let mut chars: Vec<char> = Vec::new();
    let mut lines: Vec<&str> = file_contents.split("\n").collect();

    if lines[lines.len() - 1].bytes().len() == 0 {
        lines.remove(lines.len() - 1);
    }

    let rows: usize = lines.len();
    let cols: usize = lines[0].len();

    for line in lines {
        chars.extend(line.chars().collect::<Vec<char>>())
    }

    let map = Vec2d::new(chars, rows, cols);

    let result_p1: u32 = walk_around(map);

    println!("PART 1 : {result_p1}");
}
