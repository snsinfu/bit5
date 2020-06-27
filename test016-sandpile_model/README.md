# Period of 2D Deterministic Sandpile System

https://en.wikipedia.org/wiki/Abelian_sandpile_model

```
x 1 x        x 2 x
2 4 3  --->  3 0 4
x 1 x        x 2 x
```

Here I simulate a two-dimensional finite sandpile system where a grain of sand
drops at the center of the system in each iteration. Initial state is uniform.
This system can be periodic since the simulation process is deterministic.

Some interesting observations:

- Looks like the system does not return to the initial state if the initial
  state is all-{0,1,4}. Somehow all-2 and all-3 are special.
- Looks like avalanche (repeat toppling until all vertices gets stable) does
  not change the period (if any) of system.
- Odd-sized grids smaller than 13x13 are all periodic if all-3 initialization
  is used. See [period.dat](period.dat).
- For 3x3, 5x5 and 7x7 grid the period is the same for both all-2 and all-3
  initializations. But, 9x9 grid behaves differently for all-2 and all-3
  initializations. All-3 initialized system cycles in 146248 iterations, while
  all-2 initialized one does not.
