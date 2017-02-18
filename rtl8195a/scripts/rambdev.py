import sys
import uerrno
import array
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

class RamBdev:
    SEC_SIZE = 512
    def __init__(self, blocks):
        self.data = bytearray(blocks * self.SEC_SIZE)

    #r0 = self, r1 = dst, r2 = src, r3 = len, 32bits for one copy
    @micropython.asm_thumb
    def fast_copy(r0, r1, r2, r3):
        mov(r0, 2)
        asr(r3, r0) # r2 / 4
        movw(r5, 0)
        label(loop)
        ldr(r4, [r2, 0])
        str(r4, [r1, 0])
        add(r1, 4)
        add(r2, 4)
        add(r5, r5, 1)
        cmp(r5, r3)
        ble(loop)

    def readblocks(self, n, buf):
        address_base = n * self.SEC_SIZE
        dst_addr = uctypes.addressof(buf)
        src_addr = uctypes.addressof(self.data) + address_base
        self.fast_copy(dst_addr, src_addr, self.SEC_SIZE)

    def writeblocks(self, n, buf):
        address_base = n * self.SEC_SIZE
        dst_addr = uctypes.addressof(self.data) + address_base
        src_addr = uctypes.addressof(buf)
        self.fast_copy(dst_addr, src_addr, self.SEC_SIZE)

    def ioctl(self, op, arg):
        if op == 4:  # BP_IOCTL_SEC_COUNT
            return len(self.data) // self.SEC_SIZE
        if op == 5:  # BP_IOCTL_SEC_SIZE
            return self.SEC_SIZE
