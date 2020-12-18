`gsp.hpp` provides general-purpose Gillespie simulator.

- `gsp::state` represents system state. It holds time and number of molecules.
- `gsp::reaction` models a reaction. It computes reaction rate and changeset
  for given system state.
- `gsp::simulation` runs Gillespie simulation. It holds system state and a set
  of reactions. `step()` method simulates a single reaction using Gillespie
  algorithm.

`main.cc` simulates a random reaction network consisting of 30 species. Type
following commands to compile and run the program:

```console
$ make
$ ./main > _output.dat
```

The file `_output.dat` contains simulation trajectory in tab-separated values
format. The following notebook shows some plots:

https://gist.github.com/snsinfu/c2d540d336ffd3b3800d1b9d14af7f00
