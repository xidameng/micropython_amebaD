import sys
import uctypes
try:
    import uos_vfs as uos
except ImportError:
    import uos
try:
    uos.VfsFat
except AttributeError:
    print("SKIP")
    sys.exit()

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

class RamBdev:

    SEC_SIZE = 512

    def __init__(self, blocks):
        self.data = bytearray(blocks * self.SEC_SIZE)

    def readblocks(self, n, buf):
        address_base = n * self.SEC_SIZE
        dst_addr = uctypes.addressof(buf)
        src_addr = uctypes.addressof(self.data) + address_base
        fast_copy(dst_addr, src_addr, self.SEC_SIZE)

    def writeblocks(self, n, buf):
        address_base = n * self.SEC_SIZE
        dst_addr = uctypes.addressof(self.data) + address_base
        src_addr = uctypes.addressof(buf)
        fast_copy(dst_addr, src_addr, self.SEC_SIZE)

    def ioctl(self, op, arg):
        if op == 4:  # BP_IOCTL_SEC_COUNT
            return len(self.data) // self.SEC_SIZE
        if op == 5:  # BP_IOCTL_SEC_SIZE
            return self.SEC_SIZE
