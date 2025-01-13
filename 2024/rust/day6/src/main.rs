use std::clone::Clone;
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

impl Coords {
    fn from_index(index: usize, _rows: usize, cols: usize) -> Self {
        Coords(index / cols, index % cols)
    }

    fn to_index(&self, _rows: usize, cols: usize) -> usize {
        self.1 + cols * self.0
    }
}

#[derive(Debug)]
struct Vec2d<T> {
    vec: Vec<T>,
    rows: usize,
    cols: usize,
}

impl<T: Clone> Clone for Vec2d<T> {
    fn clone(&self) -> Vec2d<T> {
        Vec2d {
            vec: self.vec.clone(),
            rows: self.rows,
            cols: self.cols,
        }
    }
}

impl<T: std::cmp::PartialEq> Vec2d<T> {
    fn new(vec: Vec<T>, rows: usize, cols: usize) -> Self {
        assert!(vec.len() == rows * cols);
        Self { vec, rows, cols }
    }

    fn from_size(rows: usize, cols: usize) -> Self {
        let vec: Vec<T> = Vec::with_capacity(rows * cols);
        Self { vec, rows, cols }
    }

    fn row(&self, row: usize) -> &[T] {
        let i = self.cols * row;
        &self.vec[i..(i + self.cols)]
    }

    fn get_by_index(&self, index: usize) -> &T {
        &self.vec[index]
    }

    fn index_by_coords(&self, coords: &Coords) -> &T {
        assert!(
            self.within_bounds(coords, 0),
            "the coordinates (rows: {}, cols: {}) are out of bounds!",
            coords.0,
            coords.1
        );
        &self.vec[coords.to_index(self.rows, self.cols)]
    }

    fn set_by_coord(&mut self, value: T, coords: &Coords) {
        assert!(
            self.within_bounds(coords, 0),
            "the coordinates (rows: {}, cols: {}) are out of bounds!",
            coords.0,
            coords.1
        );
        self.vec[coords.to_index(self.rows, self.cols)] = value;
    }

    fn find(&self, element: &T) -> Option<Coords> {
        let pos = self.vec.iter().position(|x| x == element);
        match pos {
            Some(x) => Some(Coords(x / self.rows, x % self.rows)),
            _ => None,
        }
    }

    fn within_bounds(&self, coords: &Coords, edge_width: usize) -> bool {
        (coords.0 - edge_width) != usize::MAX
            && (coords.1 - edge_width) != usize::MAX
            && (coords.0 + edge_width) < self.rows
            && (coords.1 + edge_width) < self.cols
    }
}

impl Vec2d<u8> {
    fn zeroset(&mut self) {
        for i in 0..(self.rows * self.cols) {
            self.vec[i] = 0;
        }
    }
    fn zerofill(&mut self) {
        for _i in 0..(self.rows * self.cols) {
            self.vec.push(0);
        }
    }
    fn zeroes(&mut self) {
        if self.vec.len() == 0 {
            self.zerofill();
        } else {
            self.zeroset();
        }
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

fn get_stuck(mut map: Vec2d<char>) -> u32 {
    let mut crates: u32 = 0;
    let mut been_there = Vec2d::<u8>::from_size(map.rows, map.cols);

    let start: Coords = map.find(&'^').unwrap();

    for i in 0..(map.rows * map.cols) {
        if *map.get_by_index(i) == '#' || *map.get_by_index(i) == '^' {
            continue;
        }
        been_there.zeroes();
        let c = Coords::from_index(i, map.rows, map.cols);
        map.set_by_coord('#', &c);

        let mut pos: Coords = start;
        let mut dir: Directions = Directions::N;
        let mut got_stuck: bool = false;

        while map.within_bounds(&pos, 1) {
            been_there.set_by_coord(been_there.index_by_coords(&pos) + 1, &pos);
            if *been_there.index_by_coords(&pos) == 3 {
                got_stuck = true;
                break;
            }
            match dir {
                Directions::N => {
                    while pos.0 > 0 && *map.index_by_coords(&Coords(pos.0 - 1, pos.1)) != '#' {
                        pos.0 = pos.0 - 1;
                    }
                    dir = Directions::E;
                }

                Directions::S => {
                    while pos.0 < (map.rows - 1)
                        && *map.index_by_coords(&Coords(pos.0 + 1, pos.1)) != '#'
                    {
                        pos.0 = pos.0 + 1;
                    }
                    dir = Directions::W;
                }

                Directions::E => {
                    while pos.1 < (map.cols - 1)
                        && *map.index_by_coords(&Coords(pos.0, pos.1 + 1)) != '#'
                    {
                        pos.1 = pos.1 + 1;
                    }
                    dir = Directions::S;
                }

                Directions::W => {
                    while pos.1 > 0 && *map.index_by_coords(&Coords(pos.0, pos.1 - 1)) != '#' {
                        pos.1 = pos.1 - 1;
                    }
                    dir = Directions::N;
                }
            }
        }
        crates += if got_stuck { 1 } else { 0 };
        map.set_by_coord('.', &c);
    }

    crates
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

        if !map.within_bounds(&coords_tmp, 0) {
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

    let result_p1: u32 = walk_around(map.clone());
    let result_p2: u32 = get_stuck(map);

    println!("PART 1 : {result_p1}");
    println!("PART 2 : {result_p2}");
}
