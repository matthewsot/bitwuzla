
(set-logic QF_BV)
(declare-fun s () (_ BitVec 7))
(declare-fun t () (_ BitVec 7))
(assert (not (= (bvsge s t) (bvsle t s))))
(check-sat)
(exit)