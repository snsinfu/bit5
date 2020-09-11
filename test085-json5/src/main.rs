use std::collections::HashMap;
use std::fs::File;
use std::io::Read;

use serde::{Deserialize, Serialize};

fn main() {
    let filename = "vars.json5";

    let json_text = {
        let mut file = match File::open(filename) {
            Ok(file) => file,
            Err(error) => {
                eprintln!("error: {:?}", error);
                std::process::exit(1);
            }
        };

        let mut text = String::new();

        if let Err(error) = file.read_to_string(&mut text) {
            eprintln!("error: {:?}", error);
            std::process::exit(1);
        }

        text
    };

    let vars: VarsPack = match json5::from_str(json_text.as_str()) {
        Ok(vars) => vars,
        Err(error) => {
            eprintln!("error: {:?}", error);
            std::process::exit(1);
        }
    };

    println!("Host vars:\t{:?}", vars.host_vars);
    println!("Group vars:\t{:?}", vars.group_vars);
}

// Serde schema for a JSON object.
#[derive(Debug, Deserialize, Serialize)]
#[serde(untagged)] // Need this. Otherwise treated as JSON object with keys = enunm cases.
enum Value {
    Number(f64),
    String(String),
    Array(Vec<Value>),
    Object(HashMap<String, Value>),
}

// JSON object mapping variable name to its value.
type VarDefs = HashMap<String, Value>;

// Serde schema for vars-pack JSON file.
#[derive(Debug, Deserialize, Serialize)]
struct VarsPack {
    host_vars: HashMap<String, VarDefs>,
    group_vars: HashMap<String, VarDefs>,
}
