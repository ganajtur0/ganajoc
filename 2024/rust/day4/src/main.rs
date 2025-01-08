use std::env;
use std::fs;
use std::fmt;

#[derive(Debug)]
pub enum Directions {
    NW = 0,
     N = 1,
    NE = 2,
     W = 3,
     E = 4,
    SW = 5,
     S = 6,
    SE = 7,
}

#[derive(Debug)]
#[derive(Copy)]
#[derive(Clone)]
pub struct Coords(usize, usize);

impl Coords {
    pub fn from<T: TryInto<usize>>(tup: (T, T)) -> Self {
        let t0: usize = tup.0.try_into().ok().expect("oh hell nah");
        let t1: usize = tup.1.try_into().ok().expect("oh hell nah");
        Coords(t0, t1)
    }
}

#[derive(Debug)]
pub struct Vec2d<T> {
    vec: Vec<T>,
    rows: usize,
    cols: usize,
}

impl<T> Vec2d<T> {
    pub fn new(vec: Vec<T>, rows: usize, cols: usize) -> Self {
        assert!(vec.len() == rows * cols);
        Self { vec, rows, cols }
    }

    pub fn row(&self, row: usize) -> &[T] {
        let i = self.cols * row;
        &self.vec[i..(i + self.cols)]
    }

    pub fn index(&self, row: usize, col: usize) -> &T {
        let i = self.cols * row;
        &self.vec[i + col]
    }

    pub fn index_mut(&mut self, row: usize, col: usize) -> &mut T {
        let i = self.cols * row;
        &mut self.vec[i + col]
    }

    pub fn neighbors(&self, row: usize, col: usize) -> [Option<Coords>; 8] {
        let mut indices: [Option<Coords>; 8] = [None; 8];
        let mut index: usize = 0;
        for i in -1..2 {
            for j in -1..2 {
                if !(i == 0 && j == 0) {
                    let nr: i32 = row as i32 + i;
                    let nc: i32 = col as i32 + j;
                    if nr >= 0 && (nr as usize) < self.rows &&
                       nc >= 0 && (nc as usize) < self.cols {
                        indices[index] = Some(Coords(nr as usize, nc as usize));
                    }
                    index += 1;
                }
            }
        }
        return indices;
    }

}

impl<T: std::fmt::Debug> std::fmt::Display for Vec2d<T> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let mut str = String::new();
        for i in 0..self.rows {
            if i != 0 {
                str.push_str(", ");
            }
            str.push_str(&format!("{:?}", &self.row(i)));
        }
        write!(f, "[{}]", str)
    }
}


fn main() {
    let args: Vec<String> = env::args().collect();

    let file_contents = fs::read_to_string(&args[1])
        .expect("Could not read the file.");

    let mut chars: Vec<char> = Vec::new();
    let lines: Vec<&str> = file_contents.split("\n").collect();

    let mut rows: usize = 0;
    let cols: usize = lines[0].len();

    for line in lines {

        if line.bytes().len() == 0 {
            continue;
        }

        rows += 1;

        let row: Vec<char> = line.chars().collect::<Vec<char>>();
        assert_eq!(row.len(), cols);
        chars.extend(row);
    }

    let puzzle: Vec2d<char> = Vec2d::new(chars, rows, cols);

    let mut result_p1: u32 = 0;

    for r in 0..puzzle.rows {
        for c in 0..puzzle.cols {
            if *puzzle.index(r, c) != 'X' {
                continue;
            }
            for d in 0..8 {
                let mut rc = r;
                let mut cc = c;
                let mut str_found: String = String::from("X");
                for _ in 1..4 {
                    let neighbors: [Option<Coords>; 8] = puzzle.neighbors(rc, cc);
                    match neighbors[d] {
                        Some(coords) => {
                            rc = coords.0;
                            cc = coords.1;
                            str_found.push(*puzzle.index(rc, cc));
                        },
                        _ => break,
                    }
                }
                if str_found == "XMAS" || str_found == "SAMX" {
                    result_p1 += 1;
                } 
            }
        }
    }
    println!("{result_p1}");
    
    let mut result_p2: u32 = 0;

    for r in 0..(puzzle.rows - 2) {
        for c in 0..(puzzle.cols - 2) {
            let frame = vec![
                            &(puzzle.row(r))[c..(c+3)],
                            &(puzzle.row(r+1))[c..(c+3)],
                            &(puzzle.row(r+2))[c..(c+3)]
            ];
            let mut str_mas1 = String::new();
            for i in 0..=2 {
                str_mas1.push(frame[i][i]);
            }
            let mut str_mas2 = String::new();
            for tup_i in [(2,0), (1,1), (0,2)] {
                str_mas2.push(frame[tup_i.0][tup_i.1]);
            }
            if (str_mas1 == "MAS" || str_mas1 == "SAM") &&
               (str_mas2 == "MAS" || str_mas2 == "SAM") {
                result_p2 += 1;
            }
        }
    }
    println!("{result_p2}");
}
