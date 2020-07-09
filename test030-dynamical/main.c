#include <math.h>
#include <stdio.h>


struct state {
    double t;
    double x_off;
    double x_ready;
    double x_active;
    double y;
};

struct config {
    double k_off_to_ready;
    double k_ready_to_off;
    double k_ready_to_active;
    double k_active_to_ready;
    double k_constraint;
    double k_active_to_y;
    double k_y_decay;

    double activator_amp;
    double activator_freq;

    double dt;
    long   simulation_steps;
    long   sampling_interval;
};


static void print_state(struct state const *state);
static void integrate_step(struct state *state, struct config const *config);


int main(void)
{
    struct config config = {
        .k_off_to_ready    = 0.2,
        .k_ready_to_off    = 0.1,
        .k_ready_to_active = 0.2,
        .k_active_to_ready = 0.5,
        .k_constraint      = 1.0,
        .k_active_to_y     = 2,
        .k_y_decay         = 0.5,

        .activator_amp     = 9.0,
        .activator_freq    = 1,

        .dt                = 1e-5,
        .simulation_steps  = 4000000,
        .sampling_interval = 1000
    };

    struct state state = {
        .t        = 0,
        .x_off    = 1,
        .x_ready  = 0,
        .x_active = 0,
        .y        = 0
    };

    print_state(&state);

    for (long step = 1; step <= config.simulation_steps; step++) {
        integrate_step(&state, &config);

        if (step % config.sampling_interval == 0) {
            print_state(&state);
        }
    }
}


static void print_state(struct state const *state)
{
    printf(
        "%g\t%g\t%g\t%g\t%g\n",
         state->t,
         state->x_off,
         state->x_ready,
         state->x_active,
         state->y
    );
}


static void integrate_step(struct state *state, struct config const *config)
{
    double constraint = -config->k_constraint * (
            state->x_off + state->x_ready + state->x_active - 1
    );

    double activator =
        config->activator_amp * (1 - cos(config->activator_freq * state->t)) / 2;

    double dt = config->dt;

    double dx_off = dt * (
        constraint
        - config->k_off_to_ready * state->x_off
        + config->k_ready_to_off * state->x_ready
    );

    double dx_ready = dt * (
        constraint
        + config->k_off_to_ready * state->x_off
        - config->k_ready_to_off * state->x_ready
        - config->k_ready_to_active * state->x_ready * activator
        + config->k_active_to_ready * state->x_active
    );

    double dx_active = dt * (
        constraint
        + config->k_ready_to_active * state->x_ready * activator
        - config->k_active_to_ready * state->x_active
    );

    double dy = dt * (
        config->k_active_to_y * state->x_active
        - config->k_y_decay * state->y
    );

    state->t += dt;
    state->x_off += dx_off;
    state->x_ready += dx_ready;
    state->x_active += dx_active;
    state->y += dy;
}
