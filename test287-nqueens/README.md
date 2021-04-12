# N-queens

The positioning of N queens is represented by an N-by-N boolean matrix. The
N-queens problem constrain the matrix so that: any(row), any(column),
distinct(row), distinct(column), distinct(diagonal) for all rows, columns and
diagonals (including anti-diagonals).

The following is a valid 8-queens positioning obtained by solving 8-queens.cnf.

```
. . . . X . . .
. . . . . . X .
. X . . . . . .
. . . X . . . .
. . . . . . . X
X . . . . . . .
. . X . . . . .
. . . . . X . .
```

SAT checking is instantaneous with z3 or even with a naive DPLL implementation
for small N. cryptominisat5 is overwhelmingly fast.

|   N | dpll |    z3 | cryptominisat5 |
|-----|------|-------|----------------|
|  19 |   2s | 0.03s | 0.01s          |
|  20 |  11s | 0.15s | 0.01s          |
|  21 |  34s | 0.03s | 0.01s          |
|  64 |    - |  1.6s | 0.14s          |
|  70 |    - |   39s | 0.18s          |
|  80 |    - |   35s | 0.27s          |
| 100 |    - |   35s | 0.50s          |
