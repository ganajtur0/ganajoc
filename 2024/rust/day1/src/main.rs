use std::env; 
use std::fs;

fn main() {
    let args: Vec<String> = env::args().collect();

    let filename = &args[1];

    let contents = fs::read_to_string(filename)
        .expect("error reading file");

    let lines: Vec<&str> = contents.split("\n").collect();

    let mut left: Vec<i32> = Vec::new();
    let mut right: Vec<i32> = Vec::new();

    for line in lines {
        if line.bytes().count() != 0 {
            let values: Vec<&str> = line.split("   ").collect();
            left.push(values[0].parse::<i32>().unwrap());
            right.push(values[1].parse::<i32>().unwrap());
        }
    }

    left.sort();
    right.sort();

    let mut differences: Vec<i32> = Vec::with_capacity(left.len());

    for i in 0..left.len() {
        differences.push((left[i] - right[i]).abs());
    }

    let result: i32 = differences.iter().sum();

    println!("PART 1 : {result}");

    let mut sum: i32 = 0;

    for v in left {
        sum += v * right.iter().filter(|&n| *n == v).count() as i32;
    }

    println!("PART 2 : {sum}");
}
