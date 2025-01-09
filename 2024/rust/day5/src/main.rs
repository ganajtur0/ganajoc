use std::env;
use std::fs;
use std::collections::HashMap;
use std::cmp::Ordering;

enum ReadState {
    RULES,
    PAGES,
}

fn main() {
    let args: Vec<String> = env::args().collect();

    let file_contents = fs::read_to_string(&args[1])
        .expect("Could not read file");

    let mut lines: Vec<&str> = file_contents.split("\n").collect();
    lines.remove(lines.len() - 1);

    let mut rules = HashMap::new();

    let mut read_state: ReadState = ReadState::RULES;

    let mut part1: u32 = 0;
    let mut part2: u32 = 0;

    for line in lines {
        if line.bytes().len() == 0 {
            read_state = ReadState::PAGES;
            continue;
        }

        match read_state {
            ReadState::RULES => {
                let nums: Vec<u8> = line.trim().split("|").map(|x| x.parse::<u8>().unwrap()).collect();
                if !rules.contains_key(&nums[0]) {
                    rules.insert(nums[0], Vec::new());
                }
                rules.get_mut(&nums[0]).expect("").push(nums[1]);
            },
            ReadState::PAGES => {
                let mut valid: bool = true;
                let mut pages: Vec<u8> = line.trim().split(",").map(|x| x.parse::<u8>().unwrap()).collect();
                for i in 0..(pages.len()-1) {
                    if !rules.contains_key(&pages[i]) ||
                       !rules.get(&pages[i]).expect("").contains(&pages[i+1]) {
                        valid = false;

                        // PART 2
                        pages.sort_by(|a, b| if !rules.contains_key(&a) || !rules.get(&a).expect("").contains(&b)
                            {Ordering::Less} else
                            {Ordering::Greater}
                        );
                        part2 += pages[pages.len()/2] as u32;

                        break;
                    }
                }
                part1 += if valid { pages[pages.len()/2] as u32 } else { 0 };
            },
        }
    }

    println!("PART 1 : {part1}");
    println!("PART 2 : {part2}");
}
