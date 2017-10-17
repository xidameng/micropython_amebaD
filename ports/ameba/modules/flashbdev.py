from umachine import FLASH

class FlashBdev:
    START_ADDR = 0x100000
    SEC_SIZE = 0x1000 # most flash sector is 4K
    START_SEC = START_ADDR // SEC_SIZE
    NUM_BLK = 252
    def __init__(self, blocks=NUM_BLK):
        self.blocks = blocks
        self.flash = FLASH()

    def erasesector(self, addr):
        self.flash.erase_sector(addr)
        
    def eraseblock(self, addr):
        self.flash.erase_block(addr)

    def readblocks(self, n, buf):
        src_addr = (n + self.START_SEC) * self.SEC_SIZE
        self.flash.read(buf, src_addr)

    def writeblocks(self, n, buf):
        address_base = (n + self.START_SEC) * self.SEC_SIZE
        self.erasesector((n + self.START_SEC) * self.SEC_SIZE)
        self.flash.write(buf, address_base)

    def ioctl(self, op, arg):
        if op == 4: # BP_IOCTL_SEC_COUNT
            return self.blocks
        if op == 5: # BP_IOCTL_SEC_SIZE
            return self.SEC_SIZE
