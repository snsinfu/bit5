# Enumerating solutions for two-node boolean network dynamics

We check the satisfiability and the solutions of 15-step dynamics of a two-node
boolean network. The rules consist of:

```
Negative feedback:
  a(t) a(t+1) => -b(t+1)

Strong positive feedback:
  b(t) b(t+1) => a(t+1)
  -b(t) -b(t+1) => -a(t+1)

Spontaneous activation:
  -b(t) -b(t+1) => b(t+2)

Histeresis:
  -a(t) a(t+1) => a(t+2)
  -b(t) b(t+1) => b(t+2)

Boundary condition:
  a(T) = true
  b(T) = false
```

Z3 found the following 28 distinct solutions. We observe some interesting facts:

- The system oscillates with non-uniform periods.
- The system never show a temporal pattern "1 1 1" on both nodes.
- The boundary condition fixes the last three timesteps in this system.
- There is only one solution that admits A(0) = false.

```console
$ python3 main.py
SAT
  A: 0 0 0 0 1 1 0 1 1 0 1 1 0 0 1 1
  B: 1 0 0 1 1 0 1 1 0 1 1 0 0 1 1 0
SAT
  A: 1 0 0 1 1 0 1 1 0 0 1 1 0 0 1 1
  B: 0 0 1 1 0 1 1 0 0 1 1 0 0 1 1 0
SAT
  A: 1 1 0 1 1 0 1 1 0 0 1 1 0 0 1 1
  B: 1 0 1 1 0 1 1 0 0 1 1 0 0 1 1 0
SAT
  A: 1 1 0 0 1 1 0 0 1 1 0 1 1 0 1 1
  B: 1 0 0 1 1 0 0 1 1 0 1 1 0 1 1 0
SAT
  A: 1 1 0 0 1 1 0 1 1 0 0 1 1 0 1 1
  B: 1 0 0 1 1 0 1 1 0 0 1 1 0 1 1 0
SAT
  A: 1 1 0 0 1 1 0 1 1 0 1 1 0 0 1 1
  B: 1 0 0 1 1 0 1 1 0 1 1 0 0 1 1 0
SAT
  A: 1 1 0 0 1 1 0 1 1 0 0 1 1 0 1 1
  B: 1 0 0 1 1 0 1 1 0 0 1 1 0 1 1 0
SAT
  A: 1 1 0 0 1 1 0 0 1 1 0 1 1 0 1 1
  B: 1 0 0 1 1 0 0 1 1 0 1 1 0 1 1 0
SAT
  A: 1 1 0 1 1 0 1 1 0 0 1 1 0 0 1 1
  B: 1 0 1 1 0 1 1 0 0 1 1 0 0 1 1 0
SAT
  A: 1 1 0 1 1 0 0 1 1 0 1 1 0 0 1 1
  B: 1 0 1 1 0 0 1 1 0 1 1 0 0 1 1 0
SAT
  A: 1 1 0 1 1 0 0 1 1 0 0 1 1 0 1 1
  B: 1 0 1 1 0 0 1 1 0 0 1 1 0 1 1 0
SAT
  A: 1 1 0 1 1 0 0 1 1 0 1 1 0 0 1 1
  B: 1 0 1 1 0 0 1 1 0 1 1 0 0 1 1 0
SAT
  A: 1 1 0 1 1 0 1 1 0 0 1 1 0 0 1 1
  B: 1 0 1 1 0 1 1 0 0 1 1 0 0 1 1 0
SAT
  A: 1 0 0 0 1 1 0 0 1 1 0 1 1 0 1 1
  B: 1 0 0 1 1 0 0 1 1 0 1 1 0 1 1 0
SAT
  A: 1 0 1 1 0 1 1 0 1 1 0 1 1 0 1 1
  B: 0 1 1 0 1 1 0 1 1 0 1 1 0 1 1 0
SAT
  A: 1 0 1 1 0 0 1 1 0 0 1 1 0 0 1 1
  B: 0 1 1 0 0 1 1 0 0 1 1 0 0 1 1 0
SAT
  A: 1 0 1 1 0 1 1 0 1 1 0 1 1 0 1 1
  B: 0 1 1 0 1 1 0 1 1 0 1 1 0 1 1 0
SAT
  A: 1 0 0 0 1 1 0 0 1 1 0 1 1 0 1 1
  B: 1 0 0 1 1 0 0 1 1 0 1 1 0 1 1 0
SAT
  A: 1 0 0 1 1 0 0 1 1 0 0 1 1 0 1 1
  B: - 0 1 1 0 0 1 1 0 0 1 1 0 1 1 0
SAT
  A: 1 0 0 1 1 0 1 1 0 0 1 1 0 0 1 1
  B: - 0 1 1 0 1 1 0 0 1 1 0 0 1 1 0
SAT
  A: 1 0 0 1 1 0 0 1 1 0 0 1 1 0 1 1
  B: - 0 1 1 0 0 1 1 0 0 1 1 0 1 1 0
SAT
  A: 1 0 0 1 1 0 0 1 1 0 1 1 0 0 1 1
  B: - 0 1 1 0 0 1 1 0 1 1 0 0 1 1 0
SAT
  A: 1 0 0 1 1 0 0 1 1 0 0 1 1 0 1 1
  B: - 0 1 1 0 0 1 1 0 0 1 1 0 1 1 0
SAT
  A: 1 0 0 0 1 1 0 0 1 1 0 1 1 0 1 1
  B: 1 0 0 1 1 0 0 1 1 0 1 1 0 1 1 0
SAT
  A: 1 0 0 0 1 1 0 1 1 0 0 1 1 0 1 1
  B: 1 0 0 1 1 0 1 1 0 0 1 1 0 1 1 0
SAT
  A: 1 0 0 0 1 1 0 1 1 0 1 1 0 0 1 1
  B: 1 0 0 1 1 0 1 1 0 1 1 0 0 1 1 0
SAT
  A: 1 0 0 0 1 1 0 1 1 0 0 1 1 0 1 1
  B: 1 0 0 1 1 0 1 1 0 0 1 1 0 1 1 0
SAT
  A: 1 0 0 0 1 1 0 0 1 1 0 1 1 0 1 1
  B: 1 0 0 1 1 0 0 1 1 0 1 1 0 1 1 0
```
