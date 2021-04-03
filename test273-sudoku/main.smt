; 3x3 sudoku
;
; +---+---+---+
; | a | b | c |
; +---+---+---+
; | d | e | f |
; +---+---+---+
; | g | h | i |
; +---+---+---+

; Cell variables
(declare-const a Int)
(declare-const b Int)
(declare-const c Int)
(declare-const d Int)
(declare-const e Int)
(declare-const f Int)
(declare-const g Int)
(declare-const h Int)
(declare-const i Int)

; Cell constraints
(define-fun range ((x Int) (min Int) (max Int)) Bool
            (and (>= x min) (<= x max)))

(assert (range a 1 3))
(assert (range b 1 3))
(assert (range c 1 3))
(assert (range d 1 3))
(assert (range e 1 3))
(assert (range f 1 3))
(assert (range g 1 3))
(assert (range h 1 3))
(assert (range i 1 3))

; Row constraints
(assert (distinct a b c))
(assert (distinct d e f))
(assert (distinct g h i))

; Column constraints
(assert (distinct a d g))
(assert (distinct b e h))
(assert (distinct c f i))

; Pre-filled values
(assert (= a 1))
(assert (= g 3))

; SAT
(check-sat)

; Result
(get-value (a b c d e f g h i))
