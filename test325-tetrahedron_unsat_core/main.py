from z3 import Solver, Bools, And, Or, Not, Implies, sat

# Tetrahedron topology:
#
#  d
#  |
#  a---b
#   \ / \
#    c---d
#
# unsat.

a, b, c, d = Bools("a b c d")
ab, ac, ad, bc, bd, cd = Bools("ab ac ad bc bd cd")


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
solver.add(arrow(ad, a, d))
solver.add(arrow(bc, b, c))
solver.add(arrow(bd, b, d))
solver.add(arrow(cd, c, d))
edges = [ab, ac, ad, bc, bd, cd]

result = solver.check(edges)
assert result != sat

print("unsat")
print("core:", solver.unsat_core())
