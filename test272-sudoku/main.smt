; 3x3 sudoku
;
; +---+---+---+
; | a | b | c |
; +---+---+---+
; | d | e | f |
; +---+---+---+
; | g | h | i |
; +---+---+---+

(declare-const a1 Bool)
(declare-const a2 Bool)
(declare-const a3 Bool)

(declare-const b1 Bool)
(declare-const b2 Bool)
(declare-const b3 Bool)

(declare-const c1 Bool)
(declare-const c2 Bool)
(declare-const c3 Bool)

(declare-const d1 Bool)
(declare-const d2 Bool)
(declare-const d3 Bool)

(declare-const e1 Bool)
(declare-const e2 Bool)
(declare-const e3 Bool)

(declare-const f1 Bool)
(declare-const f2 Bool)
(declare-const f3 Bool)

(declare-const g1 Bool)
(declare-const g2 Bool)
(declare-const g3 Bool)

(declare-const h1 Bool)
(declare-const h2 Bool)
(declare-const h3 Bool)

(declare-const i1 Bool)
(declare-const i2 Bool)
(declare-const i3 Bool)

; We could use the builtin (distinct) but anyway...
(define-fun only-one-of
            ((x Bool) (y Bool) (z Bool))
            Bool
            (or (and x (not y) (not z))
                (and y (not z) (not x))
                (and z (not x) (not y))))

; Cell constraints
(assert (only-one-of a1 a2 a3))
(assert (only-one-of b1 b2 b3))
(assert (only-one-of c1 c2 c3))
(assert (only-one-of d1 d2 d3))
(assert (only-one-of e1 e2 e3))
(assert (only-one-of f1 f2 f3))
(assert (only-one-of g1 g2 g3))
(assert (only-one-of h1 h2 h3))
(assert (only-one-of i1 i2 i3))

; Row uniqueness constraints
(assert (only-one-of a1 b1 c1))
(assert (only-one-of a2 b2 c2))
(assert (only-one-of a3 b3 c3))

(assert (only-one-of d1 e1 f1))
(assert (only-one-of d2 e2 f2))
(assert (only-one-of d3 e3 f3))

(assert (only-one-of g1 h1 i1))
(assert (only-one-of g2 h2 i2))
(assert (only-one-of g3 h3 c3))

; Column uniqueness constraints
(assert (only-one-of a1 d1 g1))
(assert (only-one-of a2 d2 g2))
(assert (only-one-of a3 d3 g3))

(assert (only-one-of b1 e1 h1))
(assert (only-one-of b2 e2 h2))
(assert (only-one-of b3 e3 h3))

(assert (only-one-of c1 f1 i1))
(assert (only-one-of c2 f2 i2))
(assert (only-one-of c3 f3 i3))

; Pre-filled values
(assert a1)
(assert g3)

; SAT
(check-sat)

; Result
(get-value (a1 a2 a3))
(get-value (b1 b2 b3))
(get-value (c1 c2 c3))

(get-value (d1 d2 d3))
(get-value (e1 e2 e3))
(get-value (f1 f2 f3))

(get-value (g1 g2 g3))
(get-value (h1 h2 h3))
(get-value (i1 i2 i3))
