try:
    import uos
    import umachine
    import uterminal
except ImportError as e:
    print(e)


class RAMFS:

    SEC_SIZE = 512

    def __init__(self, blocks):
        self.data = bytearray(blocks * self.SEC_SIZE)

    def readblocks(self, n, buf):
        #print("readblocks(%s, %x(%d))" % (n, id(buf), len(buf)))
        for i in range(len(buf)):
            buf[i] = self.data[n * self.SEC_SIZE + i]

    def writeblocks(self, n, buf):
        #print("writeblocks(%s, %x)" % (n, id(buf)))
        for i in range(len(buf)):
            self.data[n * self.SEC_SIZE + i] = buf[i]

    def ioctl(self, op, arg):
        #print("ioctl(%d, %r)" % (op, arg))
        if op == 4:  # BP_IOCTL_SEC_COUNT
            return len(self.data) // self.SEC_SIZE
        if op == 5:  # BP_IOCTL_SEC_SIZE
            return self.SEC_SIZE

_baudrate = 38400
_log_uart = umachine.LOGUART()
_log_uart.init(baudrate=_baudrate)
_log_uart.irq_enable(True)
_log_uart.irq_handler(uterminal.rx_post)
uterminal.dump().append(_log_uart)

print("Init LOGUART %d finished and install it to uterminal list" % _baudrate)

_flash = umachine.FLASH()

try:
    _flash_vfs = uos.VfsFat(_flash, "/flash")
except OSError:
    print("mount flash to file system failed, formating flash ...")
    uos.VfsFat.mkfs(_flash)
    _flash_vfs = uos.VfsFat(_flash, "/flash")

print("mount flash to file system done")
