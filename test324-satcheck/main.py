from z3 import Solver, Bools, And, Or, Not, Implies, sat

#        (c)  (d)
#         |    |
#  (b) -- a -- b -- (a)
#         |    |
#  (d) -- c -- d -- (c)
#         |    |
#        (a)  (b)

a, b, c, d = Bools("a b c d")
ab, ac, ba, bd, cd, ca, dc, db = Bools("ab ac ba bd cd ca dc db")


def repress(x, y):
    return And(Implies(x, Not(y)), Implies(Not(x), y))


def arrow(direction, x, y):
    forward = And(direction, repress(x, y))
    backward = And(Not(direction), repress(y, x))
    return Or(forward, backward)


solver = Solver()

solver.add(Or(a, b, c, d))
solver.add(arrow(ab, a, b))
solver.add(arrow(ac, a, c))
solver.add(arrow(ba, b, a))
solver.add(arrow(bd, b, d))
solver.add(arrow(cd, c, d))
solver.add(arrow(ca, c, a))
solver.add(arrow(dc, d, c))
solver.add(arrow(db, d, b))

if solver.check() != sat:
    print("unsat")
    exit(1)

model = solver.model()


def print_arrow(names, xy):
    x, y = names
    if model.eval(xy):
        print(x, "--|", y)
    else:
        print(x, "|--", y)


def print_state(name, x):
    if model.eval(x):
        print(f"+{name}")
    else:
        print(f"-{name}")


print("Network:")
print_arrow("ab", ab)
print_arrow("ac", ac)
print_arrow("ba", ba)
print_arrow("bd", bd)
print_arrow("cd", cd)
print_arrow("ca", ca)
print_arrow("dc", dc)
print_arrow("db", db)

print("State:")
print_state("a", a)
print_state("b", b)
print_state("c", c)
print_state("d", d)
