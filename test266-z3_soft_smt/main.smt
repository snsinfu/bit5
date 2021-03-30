; https://rise4fun.com/Z3/tutorialcontent/optimization

(declare-const a Bool)
(declare-const b Bool)
(declare-const c Bool)

; Avoid trivial solution.
(assert (or a b c))

; Negative feedback conditions.
(assert (=> a (not b)))
(assert (=> b (not c)))
(assert (=> c (not a)))

; Favor alternating pattern. These propositions cannot be satisfied as hard
; constraints.
(assert-soft (=> (not a) b) :weight 1)
(assert-soft (=> (not b) c) :weight 2)
(assert-soft (=> (not c) a) :weight 3)

(check-sat)

(get-value (a))
(get-value (b))
(get-value (c))
