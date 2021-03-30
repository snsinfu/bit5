; https://smtlib.github.io/jSMTLIB/SMTLIBTutorial.pdf

(set-option :produce-unsat-cores true)

(declare-const a Bool)
(declare-const b Bool)
(declare-const c Bool)

(assert (! (=> a (not b)) :named ab))
(assert (! (=> b (not c)) :named bc))
(assert (! (=> c (not a)) :named ca))
(assert (! (or a b c) :named nontrivial))

(check-sat)

(get-value (a))
(get-value (b))
(get-value (c))

; (get-unsat-core)
