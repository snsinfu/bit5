use std::fs::File;
use std::io::Read;

use serde::Deserialize;

fn main() {
    let filename = "input.json";

    let text = {
        let mut file = match File::open(filename) {
            Ok(file) => file,
            Err(error) => {
                eprintln!("error: {}", error);
                std::process::exit(1);
            }
        };

        let mut text = String::new();

        if let Err(error) = file.read_to_string(&mut text) {
            eprintln!("error: {}", error);
            std::process::exit(1);
        }

        text
    };

    let input: Input = match serde_json::from_str(&text) {
        Ok(result) => result,
        Err(error) => {
            eprintln!("error: {}", error);
            std::process::exit(1);
        }
    };

    for gate in input.gates {
        println!("{:?}", gate);
    }
}

#[derive(Debug, Deserialize)]
struct Input {
    gates: Vec<Gate>,
}

#[derive(Debug, Deserialize)]
#[serde(tag = "type")]
enum Gate {
    #[serde(rename = "sigmoid")]
    Sigmoid { threshold: f64 },

    #[serde(rename = "hill")]
    Hill { threshold: f64, exponent: i32 },
}
