(set-logic QF_BV)
(set-info :status sat)
(declare-const v0 (_ BitVec 5))
(assert (= #b1 (bvand (ite (= (bvmul v0 v0) (_ bv25 5)) #b1 #b0) (bvnot (bvumulo v0 v0)))))
(check-sat)