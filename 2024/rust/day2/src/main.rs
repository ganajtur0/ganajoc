use std::env;
use std::fs;

fn safe_p1(nums: &[i16]) -> bool {

    let inc: bool = (nums[1] - nums[0]) > 0;

    for i in 1..nums.len() {

        let diff: i16 = nums[i] - nums[i-1];

        if (inc && diff < 0) || (!inc && diff > 0) || diff.abs() < 1 || diff.abs() > 3 {
            return false;
        }

    }

    return true;
}

fn safe_p2(nums: &Vec<i16>) -> bool {

    let mut success = false;

    for i in 0..nums.len() {
        let skipped = [&nums[..i], &nums[(i+1)..]].concat();
        if safe_p1(skipped.as_slice()) {
            success = true;
            break;
        }
    }

    return success;
}

fn main() {

    let args: Vec<String> = env::args().collect();

    let contents = fs::read_to_string(&args[1])
        .expect("Couldn't read the file");

    let lines: Vec<&str> = contents.split("\n").collect();

    let mut result_p1: i16 = 0;
    let mut result_p2: i16 = 0;

    for line in lines {
        if line.bytes().len() != 0 {
            let nums: Vec<i16> = line.split(" ").map(|x| x.parse::<i16>().unwrap()).collect();
            if safe_p1(nums.as_slice()) {
                result_p1+=1;
            }
            if safe_p2(&nums) {
                result_p2+=1;
            }
        }
    }

    println!("PART 1 : {result_p1}");
    println!("PART 2 : {result_p2}");
}
