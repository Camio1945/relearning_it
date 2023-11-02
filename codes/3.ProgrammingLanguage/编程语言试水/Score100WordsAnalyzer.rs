use std::{fs::read_to_string, time::Instant};

fn main() {
    let start = Instant::now();
    let dictionary: String = read_to_string("Dictionary.txt").unwrap();
    let score_100_words: Vec<&str> = dictionary
        .lines()
        .filter(|line|
            !line.contains("-")
                && !line.contains(" ")
                && !line.contains("="))
        .filter(|word|
            word.chars()
                .map(|c| c as i32 - 'a' as i32 + 1)
                .sum::<i32>() == 100)
        .collect();
    println!("{:?}", score_100_words);
    println!("\nwords number: {}", score_100_words.len());
    println!("        time: {}ms", start.elapsed().as_millis());
}
