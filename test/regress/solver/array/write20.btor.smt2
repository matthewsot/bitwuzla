(set-logic QF_ABV)
(set-info :status sat)
(declare-const v0 (_ BitVec 8))
(declare-const v1 (_ BitVec 8))
(declare-const v2 (_ BitVec 8))
(declare-const v3 (_ BitVec 8))
(declare-const a0 (Array (_ BitVec 8) (_ BitVec 8) ))
(declare-const a1 (Array (_ BitVec 8) (_ BitVec 8) ))
(assert (= #b1 (bvnot (bvor (bvnot (ite (= v0 v2) #b1 #b0)) (ite (= (ite (= (store a0 v0 v1) (store a1 v2 v3)) #b1 #b0) (bvand (ite (= v1 v3) #b1 #b0) (ite (= a0 a1) #b1 #b0))) #b1 #b0)))))
(check-sat)