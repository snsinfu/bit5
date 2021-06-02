fn main() {
    let g = 2.0;
    let k = 0.5;
    let q = 0.10;//1.0;
    let r = 0.11;//1.1;
    let dt = 0.001;
    let steps = 1_000_000;
    let output_interval = 100;

    let mut x = 0.1;
    let mut y = 0.2;
    let mut z = 0.3;
    let mut phi_x = 0.1;
    let mut phi_y = 0.1;
    let mut phi_z = 0.1;
    let mut t = 0.0;

    for step in 0..=steps {
        if step % output_interval == 0 {
            println!("{:.3}\t{:.6}\t{:.6}\t{:.6}\t{:.6}\t{:.6}\t{:.6}", t, phi_x, phi_y, phi_z, x, y, z);
        }

        let x_rate = phi_x * g - k * x;
        let y_rate = phi_y * g - k * y;
        let z_rate = phi_z * g - k * z;
        let phi_x_rate = q - r * z;
        let phi_y_rate = q - r * x;
        let phi_z_rate = q - r * y;

        let dx = x_rate * dt;
        let dy = y_rate * dt;
        let dz = z_rate * dt;

        // Predictor-corrector integration. Without this hack, the differential
        // becomes so small that the system gets stuck at phi = 0 or 1.
        let pre_dphi_x = phi_x_rate * dt;
        let pre_dphi_y = phi_y_rate * dt;
        let pre_dphi_z = phi_z_rate * dt;
        let dphi_x = corrector(phi_x + pre_dphi_x) * pre_dphi_x;
        let dphi_y = corrector(phi_y + pre_dphi_y) * pre_dphi_y;
        let dphi_z = corrector(phi_z + pre_dphi_z) * pre_dphi_z;

        // XXX: Still, the predictor `phi_x + pre_dphi_x` may produce overshoot
        // prediction. This scheme would not always work...

        // This naive scheme (*) unfortunately produces a frozen system.
        // let dphi_x = corrector(phi_x) * pre_dphi_x;
        // let dphi_y = corrector(phi_y) * pre_dphi_y;
        // let dphi_z = corrector(phi_z) * pre_dphi_z;

        x += dx;
        y += dy;
        z += dz;
        phi_x += dphi_x;
        phi_y += dphi_y;
        phi_z += dphi_z;
        t += dt;

        // As for the naive scheme (*), this artificial clampping mitigates the
        // issue but is ugly.
        // let epsilon = 1e-8;
        // phi_x = clamp(phi_x, epsilon, 1. - epsilon);
        // phi_y = clamp(phi_y, epsilon, 1. - epsilon);
        // phi_z = clamp(phi_z, epsilon, 1. - epsilon);
    }
}

fn corrector(x: f64) -> f64 {
    x * (1. - x)
}
