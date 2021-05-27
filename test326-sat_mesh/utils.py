from z3 import sat
from z3 import Or


def exhaust_models(solver, variables):
    while solver.check() == sat:
        model = solver.model()
        yield model

        blocker = []
        for var in variables:
            blocker.append(var != model.eval(var))
        solver.add(Or(*blocker))
