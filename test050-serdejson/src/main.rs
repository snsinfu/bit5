use serde::{Deserialize, Serialize};

#[derive(Debug, Serialize, Deserialize)]
struct Network {
    reactants: Vec<String>,
    transitions: Vec<Transition>,
}

#[derive(Debug, Serialize, Deserialize)]
struct Transition {
    from: String,
    to: String,
    rate: f64,
}

fn main() {
    let json_text = r#"
        {
            "reactants": [
                "Gene/repressed",
                "Gene/neutral",
                "Gene/active",
                "mRNA",
                "Protein"
            ],

            "transitions": [
                {"from": "Gene/neutral", "to": "Gene/repressed", "rate": 0.5},
                {"from": "Gene/neutral", "to": "Gene/active", "rate": 0.2}
            ]
        }
    "#;

    let json: Network = match serde_json::from_str(json_text) {
        Ok(json) => json,
        Err(error) => {
            eprintln!("{}", error);
            std::process::exit(1);
        }
    };

    println!("{:#?}", json);
}
