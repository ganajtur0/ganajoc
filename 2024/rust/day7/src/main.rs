use std::env;
use std::fs;

/*
struct Calibration {
    target: u64,
    operands: Vec<u32>,
}

impl Calibration {
    fn new(target: u64, operands: Vec<u32>) -> Self {
        Calibration { target, operands }
    }
}
*/

fn obtainable(target: u64, operands: &[u32]) -> bool {
    let last: u64 = *operands.last().unwrap() as u64;
    if operands.len() == 1 {
        return target == last;
    }
    if target % last == 0 && obtainable(target / last, &operands[0..operands.len() - 1]) {
        return true;
    }
    if target > last && obtainable(target - last, &operands[0..operands.len() - 1]) {
        return true;
    }
    return false;
}

fn obtainable_deluxe(target: u64, operands: &[u32]) -> bool {
    let last: u64 = *operands.last().unwrap() as u64;
    if operands.len() == 1 {
        return target == last;
    }
    if target % last == 0 && obtainable_deluxe(target / last, &operands[0..operands.len() - 1]) {
        return true;
    }
    if target > last && obtainable_deluxe(target - last, &operands[0..operands.len() - 1]) {
        return true;
    }
    let target_str = target.to_string();
    let last_str = last.to_string();
    if target_str.ends_with(&last_str)
        && target_str.len() > last_str.len()
        && obtainable_deluxe(
            target_str[0..target_str.len() - last_str.len()]
                .parse::<u64>()
                .unwrap(),
            &operands[0..operands.len() - 1],
        )
    {
        return true;
    }
    return false;
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let file_contents = fs::read_to_string(&args[1]).expect("Could not read file");
    let mut lines: Vec<&str> = file_contents.split("\n").collect();
    lines.remove(lines.len() - 1);

    let mut part1: u128 = 0;
    let mut part2: u128 = 0;

    for line in lines {
        let tokens: Vec<&str> = line.split(": ").collect();
        let target: u64 = tokens[0].parse::<u64>().unwrap();
        let operands: Vec<u32> = tokens[1]
            .split(" ")
            .map(|x| x.parse::<u32>().unwrap())
            .collect();
        // let cal: Calibration = Calibration::new(target, operands);
        if obtainable(target, &operands) {
            part1 += target as u128;
        }
        if obtainable_deluxe(target, &operands) {
            part2 += target as u128;
        }
    }

    println!("PART 1 : {part1}");
    println!("PART 2 : {part2}");
}
