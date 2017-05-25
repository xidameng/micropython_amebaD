import sys
import uerrno
import array
import uctypes
from utils import fast_copy
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
        #print("ioctl(%d, %r)" % (op, arg))
        if op == 4:  # BP_IOCTL_SEC_COUNT
            return len(self.data) // self.SEC_SIZE
        if op == 5:  # BP_IOCTL_SEC_SIZE
            return self.SEC_SIZE
