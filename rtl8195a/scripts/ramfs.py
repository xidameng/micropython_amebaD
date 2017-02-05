import sys
import uerrno
try:
    import uos_vfs as uos
except ImportError:
    import uos
try:
    uos.VfsFat
except AttributeError:
    print("SKIP")
    sys.exit()

class RAMFS:

    SEC_SIZE = 512

    def __init__(self, blocks):
        self.data = bytearray(blocks * self.SEC_SIZE)

    def readblocks(self, n, buf):
        mv = memoryview(buf)
        for i in range(len(buf)):
            mv[i] = self.data[n * self.SEC_SIZE + i]

    def writeblocks(self, n, buf):
        mv = memoryview(buf)
        for i in range(len(buf)):
            self.data[n * self.SEC_SIZE + i] = mv[i]

    def ioctl(self, op, arg):
        #print("ioctl(%d, %r)" % (op, arg))
        if op == 4:  # BP_IOCTL_SEC_COUNT
            return len(self.data) // self.SEC_SIZE
        if op == 5:  # BP_IOCTL_SEC_SIZE
            return self.SEC_SIZE
