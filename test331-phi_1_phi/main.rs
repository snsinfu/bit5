fn main() {
    // d/dt phi = phi (1 - phi) (r - h x)
    // d/dt x = g phi (a + x) - k x

    let r = 1.0;
    let h = 0.2;
    let g = 0.2;
    let a = 1.0;
    let k = 0.1;
    let dt = 0.001;

    let steps = 1_000_000;
    let sampling_interval = 100;

    let mut phi = 0.3;
    let mut x = 0.0;

    let output = |step, phi, x| {
        println!("{}\t{:.5}\t{:.3}", step, phi, x);
    };

    output(0, phi, x);

    for step in 1..=steps {
        let delta_phi = dt * phi * (1. - phi) * (r - h * x);
        let delta_x = dt * (g * phi * (a + x) - k * x);

        phi += delta_phi;
        x += delta_x;

        if step % sampling_interval == 0 {
            output(step, phi, x);
        }
    }
}
