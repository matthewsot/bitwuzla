(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 8))
(declare-const v1 (_ BitVec 8))
(assert (= #b1 (bvor (bvor (bvnot (bvand (ite (= v0 (bvxor v1 (bvxor v0 v1))) #b1 #b0) (ite (= v1 (bvxor (bvxor v0 v1) (bvxor v1 (bvxor v0 v1)))) #b1 #b0))) (bvnot (bvand (ite (= v0 (bvsub (bvadd v0 v1) v1)) #b1 #b0) (ite (= v1 (bvsub (bvadd v0 v1) (bvsub (bvadd v0 v1) v1))) #b1 #b0)))) (bvor (bvnot (bvand (ite (= v0 (bvadd v1 (bvsub v0 v1))) #b1 #b0) (ite (= v1 (bvsub (bvadd v1 (bvsub v0 v1)) (bvsub v0 v1))) #b1 #b0))) (bvnot (bvand (ite (= v0 (bvsub v1 (bvsub v1 v0))) #b1 #b0) (ite (= v1 (bvadd (bvsub v1 v0) (bvsub v1 (bvsub v1 v0)))) #b1 #b0)))))))
(check-sat)