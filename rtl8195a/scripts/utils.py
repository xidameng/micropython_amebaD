@micropython.asm_thumb
def fast_copy(r0, r1, r2):
    movw(r3, 0)
    label(loop)
    ldr(r4, [r1, 0])
    str(r4, [r0, 0])
    add(r0, 1)
    add(r1, 1)
    add(r3, r3, 1)
    cmp(r3, r2)
    ble(loop)
