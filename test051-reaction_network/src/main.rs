use std::fs::File;
use std::io::Read;

use serde::{Deserialize, Serialize};

fn main() {
    //
    // Load network configuration.
    //
    let filename = "network.json";

    let network_text = {
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

    let network: Network = match serde_json::from_str(network_text.as_str()) {
        Ok(network) => network,
        Err(error) => {
            eprintln!("error: {:}", error);
            std::process::exit(1);
        }
    };

    //
    // Run simulation.
    //
    let mut state: Vec<f64> = vec![0.0; network.reactants];
    let mut influx: Vec<f64> = vec![0.0; network.reactants];

    state[0] = 1.0; // FIXME: Hard-coded
    let steps = 30000; // FIXME: Hard-coded
    let dt = 1e-3; // FIXME: Hard-coded

    for step in 0..=steps {
        // Print state.

        print!("{}", step);
        for value in &state {
            print!("\t{:.6}", value);
        }
        println!("");

        // Compute state change.
        for value in &mut influx {
            *value = 0.0;
        }

        for trans in &network.transitions {
            let flux = trans.rate * state[trans.reactant];
            influx[trans.product] += flux;
            influx[trans.reactant] -= flux;
        }

        for trans in &network.mediated_transitions {
            let gate = mediate(state[trans.mediator], trans.threshold);
            let flux = gate * trans.rate * state[trans.reactant];
            influx[trans.product] += flux;
            influx[trans.reactant] -= flux;
        }

        for gen in &network.generations {
            influx[gen.product] += gen.rate * state[gen.source];
        }

        for decay in &network.decays {
            influx[decay.reactant] -= decay.rate * state[decay.reactant];
        }

        // Apply state change.
        for i in 0..state.len() {
            state[i] += dt * influx[i];
        }
    }
}

fn mediate(x: f64, thresh: f64) -> f64 {
    let exp = 4; // FIXME: Hard-coded
    let u = (x / thresh).powi(exp);

    u / (1.0 + u)
}

#[derive(Deserialize, Serialize)]
struct Network {
    reactants: usize,
    transitions: Vec<Transition>,
    mediated_transitions: Vec<MediatedTransition>,
    generations: Vec<Generation>,
    decays: Vec<Decay>,
}

#[derive(Deserialize, Serialize)]
struct Transition {
    reactant: usize,
    product: usize,
    rate: f64,
}

#[derive(Deserialize, Serialize)]
struct MediatedTransition {
    mediator: usize,
    reactant: usize,
    product: usize,
    threshold: f64,
    rate: f64,
}

#[derive(Deserialize, Serialize)]
struct Generation {
    source: usize,
    product: usize,
    rate: f64,
}

#[derive(Deserialize, Serialize)]
struct Decay {
    reactant: usize,
    rate: f64,
}
