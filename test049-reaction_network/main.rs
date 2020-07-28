// Consider a reaction network:
//
//      OFF <---> NEUTRAL (N) <---> ON
//
// The OFF and ON states are self-reinforcing; that is, the reaction N->OFF/ON
// accelerates as the density of OFF/ON state increases.
//
// TODO: Observe histeresis.

fn main() {
    const OFF: usize = 0;
    const NEUT: usize = 1;
    const ON: usize = 2;

    let spontaneous_transitions = [
        // (from, to, rate)
        (NEUT, OFF, 0.3),
        (OFF, NEUT, 0.5),
        (NEUT, ON, 0.3),
        (ON, NEUT, 0.5),
    ];

    let mediated_transitions = [
        // (from, mediator, to, threshold, rate)
        (NEUT, OFF, OFF, 0.2, 2.5),
        (NEUT, ON, ON, 0.2, 1.2),
        (OFF, ON, NEUT, 0.3, 1.0),
        (ON, OFF, NEUT, 0.5, 2.0),
    ];

    // System converges to OFF or ON state depending on the initial state.
    let mut state: [f64; 3] = [0.0, 0.9, 0.1];

    let dt = 0.001;
    let steps = 15000;

    for step in 0..=steps {
        let time = (step as f64) * dt;

        println!("{:.3}\t{:.6}\t{:.6}\t{:.6}", time, state[0], state[1], state[2]);

        // Compute next state.
        let mut deltas: [f64; 3] = [0.0, 0.0, 0.0];

        for &(from, to, rate) in &spontaneous_transitions {
            let delta = rate * state[from];
            deltas[from] -= delta;
            deltas[to] += delta;
        }

        for &(from, med, to, threshold, rate) in &mediated_transitions {
            let delta = rate * activation(state[med], threshold) * state[from];
            deltas[from] -= delta;
            deltas[to] += delta;
        }

        for i in 0..3 {
            state[i] += dt * deltas[i];
        }
    }
}

fn activation(x: f64, thresh: f64) -> f64 {
    let exp = 4;
    let eps = 1e-6;

    1.0 / (1.0 + (thresh / (x + eps)).powi(exp))
}
