from z3 import *
from utils import all_smt

T = 15

solver = Solver()

a = [Bool(f"a{i}") for i in range(T + 1)]
b = [Bool(f"b{i}") for i in range(T + 1)]
terms = a + b

# Regulation: a(t) a(t+1) => -b(t+1)
for t in range(T):
    solver.add(Implies(And(a[t], a[t + 1]), Not(b[t + 1])))

# Regulation: b(t) b(t+1) => a(t+1)
for t in range(T):
    solver.add(Implies(And(b[t], b[t + 1]), a[t + 1]))

# Anti-regulation: -b(t) -b(t+1) => -a(t+1)
for t in range(T):
    solver.add(Implies(And(Not(b[t]), Not(b[t + 1])), Not(a[t + 1])))

# Spontaneous activation: -b(t) -b(t+1) => b(t+2)
for t in range(T - 1):
    solver.add(Implies(And(Not(b[t]), Not(b[t + 1])), b[t + 2]))

# Histeresis: -x(t) x(t+1) => x(t+2)  for both x = a, b
for t in range(T - 1):
    solver.add(Implies(And(Not(a[t]), a[t + 1]), a[t + 2]))
    solver.add(Implies(And(Not(b[t]), b[t + 1]), b[t + 2]))

# Final condition
solver.add(a[-1])
solver.add(Not(b[-1]))


def fmt(x):
    if is_true(x):
        return "1"
    if is_false(x):
        return "0"
    return "-"


for model in all_smt(solver, terms):
    print("SAT")
    a_hist = " ".join(fmt(model.eval(at)) for at in a)
    b_hist = " ".join(fmt(model.eval(bt)) for bt in b)
    print("  A:", a_hist)
    print("  B:", b_hist)
