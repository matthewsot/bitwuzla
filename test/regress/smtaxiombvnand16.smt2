(set-logic QF_BV)
(declare-fun s () (_ BitVec 16))
(declare-fun t () (_ BitVec 16))
(assert (not (= (bvnand s t) (bvnot (bvand s t)))))
(check-sat)
(exit)