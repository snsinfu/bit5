# Ising model with interaction matrix
#
#   J = [ 0 -1 ]
#       [ 1  0 ] .
#
# A spin state is represented by a pair (θi, pi) of phase θi and momentum pi.
# The spin direction is defined to be cos θ. Now we would like to define the
# classical Hamiltonian, but substituting our J into
#
#   H = ∑[i] 1/2 pi^2 - ∑[i,j] Jij cos θi cos θj
#
# results in a trivial "ideal gas" Hamiltonian:
#
#   H = ∑[i] 1/2 pi^2
#
# due to the assymmetry of the interaction matrix J.
#
# Here I introduce auxiliary variables that represent local field around the
# spins, and write the potential energy part of the hamiltonian as
#
#   V = -∑[i] ηi cos θi + ∑[i] K/2 (ηi - ∑[j] Jij cos θj)^2 .
#
# For our two-spin system, the equations reduce to
#
#   V = −ηa cos θa − ηb cos θb + K/2 (ηa + J cos θb)^2 + K/2 (ηb − J cos θa)^2
#
#   Fθa = −∂V/∂θa = −(ηa + K (ηb − J cos θa) J) sin θa
#   Fθb = −∂V/∂θb = −(ηb − K (ηa + J cos θb) J) sin θb
#   Fηa = −∂V/∂ηa = cos θa − K (ηa + J cos θb)
#   Fηb = −∂V/∂ηb = cos θb − K (ηb − J cos θa)
#
# The equations get ugly due to reaction forces... but anyway.


from math import sin, cos


#
# Parameters
#

phase_a, velocity_a = 0, 0.1
phase_b, velocity_b = 3, 0.1
field_a, vfield_a = 1, 0
field_b, vfield_b = 1, 0

interaction = 2.0
k = 1.0


time_delta = 0.005
steps = 10000
output_interval = 10

#
# Simulation
#

def output(step):
    time = step * time_delta
    spin_a = cos(phase_a)
    spin_b = cos(phase_b)

    row = [
        f"{time:g}",
        f"{spin_a:.2f}",
        f"{spin_b:.2f}",
        f"{field_a:g}",
        f"{field_b:g}",
    ]
    print("\t".join(row))


output(0)

for step in range(1, steps + 1):
    # Compute "force"
    force_a = -(field_a + k * (field_b - interaction * cos(phase_a))) * sin(phase_a)
    force_b = -(field_b - k * (field_a + interaction * cos(phase_b))) * sin(phase_b)
    force_fa = cos(phase_a) - k * (field_a + interaction * cos(phase_b))
    force_fb = cos(phase_b) - k * (field_b + interaction * cos(phase_a))

    # Implicit-Euler integration
    velocity_a += force_a * time_delta
    velocity_b += force_b * time_delta
    vfield_a += force_fa * time_delta
    vfield_b += force_fb * time_delta

    phase_a += velocity_a * time_delta
    phase_b += velocity_b * time_delta
    field_a += vfield_a * time_delta
    field_b += vfield_b * time_delta

    # Output
    if step % output_interval == 0:
        output(step)
