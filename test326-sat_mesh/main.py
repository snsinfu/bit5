# Square lattice with the torus topology.
#
#       (g)  (h)  (i)
#        |    |    |
# (c) -- a -- b -- c -- (a)
#        |    |    |
# (f) -- d -- e -- f -- (d)
#        |    |    |
# (i) -- g -- h -- i -- (g)
#        |    |    |
#       (a)  (b)  (c)
#
# UNSAT if the size of the unit cell (3 in the above example) is odd.

import argparse
import signal
import sys

import numpy as np
import z3

from z3 import Solver, Bool, And, Or, Not, Implies
from utils import exhaust_models


# CLI

signal.signal(signal.SIGINT, signal.SIG_DFL)
signal.signal(signal.SIGPIPE, signal.SIG_DFL)

parser = argparse.ArgumentParser()
parser.add_argument("size", type=int, nargs="?", default=3)
args = parser.parse_args()


# Variables

size = args.size

nodes = np.empty((size, size), dtype=object)
edges = np.empty((size, size, 2), dtype=object)

for i in range(size):
    for j in range(size):
        nodes[i, j] = Bool(f"{i}.{j}")
        edges[i, j, 0] = Bool(f"R:{i}.{j}")
        edges[i, j, 1] = Bool(f"B:{i}.{j}")

variables = list(nodes.ravel()) + list(edges.ravel())

solver = Solver()


# Repression logic


def repress(x, y):
    return And(Implies(x, Not(y)), Implies(Not(x), y))


def arrow(direction, x, y):
    forward = And(direction, repress(x, y))
    backward = And(Not(direction), repress(y, x))
    return Or(forward, backward)


for i in range(size):
    for j in range(size):
        x = nodes[i, j]
        y = nodes[(i + 1) % size, j]
        solver.add(arrow(edges[i, j, 0], x, y))

        x = nodes[i, j]
        y = nodes[i, (j + 1) % size]
        solver.add(arrow(edges[i, j, 1], x, y))


# Constraints

for i in range(size):
    for j in range(size):
        top = edges[(i + size - 1) % size, j, 1]
        left = edges[i, (j + size - 1) % size, 0]
        right = edges[i, j, 0]
        bottom = edges[i, j, 1]

        # At least one input
        solver.add(Or(top, left, Not(right), Not(bottom)))

        # At least one output
        solver.add(Or(Not(top), Not(left), right, bottom))


# Solution


def node_state(node):
    if z3.is_true(node):
        return "1"
    if z3.is_false(node):
        return "0"
    return "?"


def horizontal_arrow(edge):
    if z3.is_true(edge):
        return ">"
    if z3.is_false(edge):
        return "<"
    return "-"


def vertical_arrow(edge):
    if z3.is_true(edge):
        return "v"
    if z3.is_false(edge):
        return "^"
    return "|"


def print_model(model):
    def put(s):
        print(s, end="")

    for i in range(size):
        for j in range(size):
            put(" ")
            put(node_state(model.eval(nodes[i, j])))

            put(" ")
            put(horizontal_arrow(model.eval(edges[i, j, 0])))

        put("\n")

        for j in range(size):
            put(" ")
            put(vertical_arrow(model.eval(edges[i, j, 1])))
            put("  ")
        put("\n")


num_models = 0

for model in exhaust_models(solver, variables):
    print("sat")
    print_model(model)
    num_models += 1

if num_models == 0:
    print("unsat")
