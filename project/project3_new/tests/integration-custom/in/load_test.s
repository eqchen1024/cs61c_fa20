addi s0, x0, 200
lui  s1, 86627   # 0x15263748
addi s1, s1, 1864
sw   s1, 80(s0)
sw   s1, -20(s0)

lw   t0, 80(s0)
lh   t0, 80(s0)
lh   t1, 82(s0)
lb   t0, 80(s0)
lb   t1, 81(s0)
lb   t2, 82(s0)
lb   t0, 83(s0)

lw   t0, 92(s0)
lh   t0, 92(s0)
lh   t1, 92(s0)
lb   t0, 92(s0)
lb   t1, 92(s0)
lb   t2, 92(s0)
lb   t0, 92(s0)
