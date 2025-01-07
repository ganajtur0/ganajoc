use std::env;
use std::fs;
use regex::Regex;

fn main() {
    let args: Vec<String> = env::args().collect();

    let file_raw = fs::read_to_string(&args[1])
        .expect("Could not read the file");

    let contents: &str = file_raw.trim();

    let re = Regex::new(r"mul\((\d+,\d+)\)").unwrap();

    let mut result_p1: u32 = 0;

    for (_, [mul]) in re.captures_iter(contents).map(|c| c.extract()) {
        let operands: Vec<u32> = mul.split(",").map(|n| n.parse::<u32>().unwrap()).collect();
        result_p1 += operands[0] * operands[1];
    }
 
    println!("PART 1 : {result_p1}");

    let mut result_p2: u32 = 0;
    let mut allowed = true;

    let reg = Regex::new(r"don't\(\)|do\(\)|mul\(\d+,\d+\)").unwrap();
    let reg_mul = Regex::new(r"mul\((\d+),(\d+)\)").unwrap();

    for inst in reg.find_iter(contents).map(|m| m.as_str()) {
        if inst == "don't()" {
            allowed = false;
        }
        else if inst == "do()" {
            allowed = true; 
        }
        else if allowed {
            let (_, [op1, op2]) = reg_mul.captures(inst).unwrap().extract();
            result_p2 += op1.parse::<u32>().unwrap() * op2.parse::<u32>().unwrap();
        }
    }

    println!("PART 2 : {result_p2}");

}
