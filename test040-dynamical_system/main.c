#include <stdio.h>


struct config {
    double k_A_off_to_on;
    double k_A_on_to_off;
    double k_A_on_to_active;
    double k_A_active_to_on;

    double k_B_off_to_on;
    double k_B_on_to_off;
    double k_B_on_to_active;
    double k_B_active_to_on;

    double k_mA_generation;
    double k_mA_degradation;
    double k_mA_activates_B;

    double k_mB_generation;
    double k_mB_degradation;
    double k_mB_closes_A;

    double k_constraint;
    double timestep;

    long simulation_steps;
    long sampling_interval;
    long logging_interval;
};

struct state {
    double A_off;
    double A_on;
    double A_active;

    double B_off;
    double B_on;
    double B_active;

    double mA;
    double mB;
};

void simulate_step(struct state *state, struct config const *config);
void log_progress(long step, struct state const *state);
void save_sample(long step, struct state const *state);


int main(void)
{
    struct config config = {
        .k_A_off_to_on     = 0.1,
        .k_A_on_to_off     = 0,
        .k_A_on_to_active  = 0.2,
        .k_A_active_to_on  = 0.5,

        .k_B_off_to_on     = 0.2,
        .k_B_on_to_off     = 0.1,
        .k_B_on_to_active  = 0,
        .k_B_active_to_on  = 0.1,

        .k_mA_generation   = 0.8,
        .k_mA_degradation  = 0.3,
        .k_mA_activates_B  = 0.3,

        .k_mB_generation   = 0.2,
        .k_mB_degradation  = 0.1,
        .k_mB_closes_A     = 0.9,

        .k_constraint      = 0,

        .timestep          = 1e-4,
        .simulation_steps  = 1000000,
        .sampling_interval = 100,
        .logging_interval  = 10000
    };

    struct state state = {
        .A_off    = 1,
        .A_on     = 0,
        .A_active = 0,
        .B_off    = 1,
        .B_on     = 0,
        .B_active = 0,
        .mA       = 0,
        .mB       = 0,
    };

    log_progress(0, &state);
    save_sample(0, &state);

    for (long step = 1; step <= config.simulation_steps; step++) {
        if (step % config.logging_interval == 0) {
            log_progress(step, &state);
        }
        if (step % config.sampling_interval == 0) {
            save_sample(step, &state);
        }
        simulate_step(&state, &config);
    }
}

void log_progress(long step, struct state const *state)
{
    fprintf(stderr, "%ld\t%g\t%g\n", step, state->mA, state->mB);
}

void save_sample(long step, struct state const *state)
{
    printf(
        "%ld\t%g\t%g\t%g\t%g\t%g\t%g\t%g\t%g\n",
        step,
        state->A_off,
        state->A_on,
        state->A_active,
        state->B_off,
        state->B_on,
        state->B_active,
        state->mA,
        state->mB
    );
}

void simulate_step(struct state *state, struct config const *config)
{
    // State transitions of A.

    double F_A_off =
        - config->k_A_off_to_on * state->A_off
        + config->k_A_on_to_off * state->A_on
        + config->k_mB_closes_A * state->A_on * state->mB;

    double F_A_on =
        + config->k_A_off_to_on * state->A_off
        - config->k_A_on_to_off * state->A_on
        - config->k_mB_closes_A * state->A_on * state->mB
        - config->k_A_on_to_active * state->A_on
        + config->k_A_active_to_on * state->A_active;

    double F_A_active =
        + config->k_A_on_to_active * state->A_on
        - config->k_A_active_to_on * state->A_active;

    double F_mA =
        + config->k_mA_generation * state->A_active
        - config->k_mA_degradation * state->mA;

    // State transitions of B.

    double F_B_off =
        - config->k_B_off_to_on * state->B_off
        + config->k_B_on_to_off * state->B_on;

    double F_B_on =
        + config->k_B_off_to_on * state->B_off
        - config->k_B_on_to_off * state->B_on
        - config->k_B_on_to_active * state->B_on
        - config->k_mA_activates_B * state->B_on * state->mA
        + config->k_B_active_to_on * state->B_active;

    double F_B_active =
        + config->k_B_on_to_active * state->B_on
        + config->k_mA_activates_B * state->B_on * state->mA
        - config->k_B_active_to_on * state->B_active;

    double F_mB =
        + config->k_mB_generation * state->B_active
        - config->k_mB_degradation * state->mB;

    // Constraint terms that ensure switch states are normalized.

    double F_A_constraint = -config->k_constraint * (
        state->A_off + state->A_on + state->A_active - 1
    );

    double F_B_constraint = -config->k_constraint * (
        state->B_off + state->B_on + state->B_active - 1
    );

    // Integrate.

    double dt = config->timestep;

    state->A_off += dt * (F_A_off + F_A_constraint);
    state->A_on += dt * (F_A_on + F_A_constraint);
    state->A_active += dt * (F_A_active + F_A_constraint);

    state->B_off += dt * (F_B_off + F_B_constraint);
    state->B_on += dt * (F_B_on + F_B_constraint);
    state->B_active += dt * (F_B_active + F_B_constraint);

    state->mA += dt * F_mA;
    state->mB += dt * F_mB;
}
