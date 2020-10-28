use rand::SeedableRng;
use rand_distr::{Bernoulli, Distribution, Uniform};
use rand_pcg::Pcg64;

fn main() {
    let config = Config {
        fluctuation: 0.5,
        prob_a: 0.9,
        prob_b: 0.7,
        power_a: 0.8,
        power_b: 1.0,
        target: 1.0,
        samples: 1000,
        seed: 1,
    };

    let stats = run_simulation(&config);

    println!("{}", stats.mean_output);
    println!("{}", stats.mean_error);
}

struct Config {
    fluctuation: f64,
    prob_a: f64,
    prob_b: f64,
    power_a: f64,
    power_b: f64,
    target: f64,
    samples: i32,
    seed: u64,
}

struct Stats {
    mean_output: f64,
    mean_error: f64,
}

fn run_simulation(config: &Config) -> Stats {
    let mut random = Pcg64::seed_from_u64(config.seed);
    let fluctuation = Uniform::new_inclusive(1.0 - config.fluctuation, 1.0);
    let antenna_a = Bernoulli::new(config.prob_a).unwrap();
    let antenna_b = Bernoulli::new(config.prob_b).unwrap();

    let mut sum_sqerr = 0.0;
    let mut sum_output = 0.0;

    for _ in 0..config.samples {
        let mut output = 0.0;

        let a_on = antenna_a.sample(&mut random);
        let b_on = antenna_b.sample(&mut random);
        let input = fluctuation.sample(&mut random);

        if a_on {
            output += input * config.power_a;
        }

        if b_on {
            output += input * config.power_b;
        }

        let err = output - config.target;

        sum_output += output;
        sum_sqerr += err * err;
    }

    Stats {
        mean_output: sum_output / config.samples as f64,
        mean_error: (sum_sqerr / config.samples as f64).sqrt(),
    }
}
