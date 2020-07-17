Dynamical system consisting of two switches: `A` and `B`. Each switch changes
its internal state among `off`, `on` and `active` following a master equation.
When a switch is `active` it produces interaction agent `mX` (X = A or B).
`mA` causes transition `B:on -> B:active`. Meanwhile, `mB` causes transition
`A:on -> A:off`, thus forming a negative feedback loop. So, the system would
oscillate, but how this three-state and indirect setting affect the oscillation?

```
  A -------+
  | off    |           B -------+
  | on   (-)<------+   | off    |
  | active-----+   |   | on   (+)<----+
  +--------+   |   |   | active----+  |
               |   |   +--------+  |  |
               |   |               |  |
               |   +- mB <---------+  |
               |                      |
               +--> mA ---------------+
```

When the interaction agents are involved in the master equation linearly, the
trajectory is not that interesting. The system just converges to an attractor.

![Trajectory when interaction is linear](trajectory_linear.png)

However, if the interaction is non-linear, as seen in the `activation` function
in [the source code](main.c), the system starts oscillating! Interesting.

![Trajectory when interaction is non-linear](trajectory_cyclic.png)
