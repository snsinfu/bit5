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

from math import sin, cos


#
# Parameters
#

phase_a, velocity_a = 0, 0.1
phase_b, velocity_b = 0, 0.1
interaction = 2.0

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
        f"{velocity_a:g}",
        f"{velocity_b:g}",
    ]
    print("\t".join(row))


output(0)

for step in range(1, steps + 1):
    # Compute "force"
    force_a = sin(phase_a) * -interaction * cos(phase_b)
    force_b = sin(phase_b) * +interaction * cos(phase_a)

    # Implicit-Euler integration
    velocity_a += force_a * time_delta
    velocity_b += force_b * time_delta
    phase_a += velocity_a * time_delta
    phase_b += velocity_b * time_delta

    # Output
    if step % output_interval == 0:
        output(step)
