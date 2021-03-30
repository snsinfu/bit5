# Z3 SMT soft constraints

Use `assert-soft` with `:weight` argument to declare soft constraints. The
problem we would like to solve here is a negative feedback loop of three
boolean elements a, b and c:

```
... c ---> a ---> b ---> c ---> a ...
```

We impose hard constraints to ensure the negative feedback; that is, `a = true`
implies `b = false` etc. We also impose soft constraints to disfavor conditions
like `(a, b) = (false, false)` so that alternating pattern `(a, b) = (false, true)`
is favored.

In [main.smt](main.smt) we apply non-uniform weights (costs) to the disfavored
conditions. The solver ([z3][z3]) finds a solution that minimizes the total
weight. The result:

```console
$ z3 main.smt
sat
((a false))
((b false))
((c true))
```

[z3]: https://github.com/Z3Prover/z3
