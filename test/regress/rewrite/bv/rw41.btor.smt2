(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 8))
(assert (= #b1 (bvnot (ite (= (bvmul (bvadd (_ bv7 8) (bvadd v0 (_ bv7 8))) (_ bv5 8)) (bvadd (bvadd (bvmul (_ bv7 8) (_ bv5 8)) (bvmul (_ bv5 8) v0)) (bvmul (_ bv7 8) (_ bv5 8)))) #b1 #b0))))
(check-sat)