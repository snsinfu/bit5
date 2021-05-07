#!/bin/sh -eu

echo "=== SAT ==="

awk '
NR == 1 {
    print "p", "cnf", $3, $4
}
NR > 1 {
    $1 = ""
    print
}
' main.wcnf | z3 -in -dimacs -model

echo "=== Max (?) SAT ==="

z3 -wcnf -model main.wcnf
