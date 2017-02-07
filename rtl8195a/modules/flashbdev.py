try:
    import ameba
    import time
    import uio as io
    import array
    import uctypes
    from utils import fast_copy
except:
    raise ImportError("ameba module import failed")

PERI_ON_BASE = const(0x40000000)
REG_CPU_PERIPHERAL_CTRL = const(0x02C0)
SPI_FLASH_BASE = const(0x98000000)
SPI_FLASH_CTRL_BASE = const(0x40006000)

REG_SPIC_CTRLR0 = const(0x0000)
REG_SPIC_CTRLR1 = const(0x0004)
REG_SPIC_SSIENR = const(0x0008)
REG_SPIC_SR = const(0x0028)
REG_SPIC_DR0 = const(0x0060)
REG_SPIC_ADDR_LENGTH = const(0x0118)

class FlashBdev:
    START_ADDR = 0xb0000
    SEC_SIZE = 0x1000 # most flash sector is 4K
    START_SEC = START_ADDR // SEC_SIZE
    NUM_BLK = 50
    def __init__(self, blocks=NUM_BLK):
        self.blocks = blocks
        self.cache_buffer = bytearray(self.SEC_SIZE)

    def cmd(self, cmd, data=None, rxbuf=None):
        if len(cmd) != 1:
            raise ValueError('cmd length should be 1 byte')

        tmod = 1 if rxbuf is None else 3

        # enable CPU's spiflash pin
        ameba.mem32[PERI_ON_BASE + REG_CPU_PERIPHERAL_CTRL] |= (1<<0)
        ameba.mem32[SPI_FLASH_CTRL_BASE + REG_SPIC_SSIENR] &= ~(1<<0)
        if tmod == 3:
            ameba.mem32[SPI_FLASH_CTRL_BASE + REG_SPIC_CTRLR1] = len(rxbuf)
        if data is not None:
            ameba.mem32[SPI_FLASH_CTRL_BASE + REG_SPIC_ADDR_LENGTH] = 0 if data is None else len(data)

        ctrlr0 = ameba.mem32[SPI_FLASH_CTRL_BASE + REG_SPIC_CTRLR0]
        ctrlr0 &= 0xFFC0FCFF
        ctrlr0 |= ((tmod << 8) | (3 << 20))
        ameba.mem32[SPI_FLASH_CTRL_BASE + REG_SPIC_CTRLR0] = ctrlr0

        ameba.mem8[SPI_FLASH_CTRL_BASE + REG_SPIC_DR0] = int.from_bytes(cmd, 1)

        if data is not None:
            for i in data:
                ameba.mem8[SPI_FLASH_CTRL_BASE + REG_SPIC_DR0] = i

        # activate this transfer
        ameba.mem32[SPI_FLASH_CTRL_BASE + REG_SPIC_SSIENR] |= (1<<0)

        counter = 1000000
        while counter:
            reg_sr = ameba.mem32[SPI_FLASH_CTRL_BASE + REG_SPIC_SR]
            busy = reg_sr & (1<<0)
            if busy == 0:
                break
            counter -= 1

        ameba.mem32[SPI_FLASH_CTRL_BASE + REG_SPIC_SSIENR] &= ~(1<<0)

        if rxbuf is not None:
            for i in range(len(rxbuf)):
                rxbuf[i] = ameba.mem8[SPI_FLASH_CTRL_BASE + REG_SPIC_DR0]

        ameba.mem32[SPI_FLASH_CTRL_BASE + REG_SPIC_SSIENR] &= ~(1<<0)
        # disable CPU's spiflash pin
        ameba.mem32[PERI_ON_BASE + REG_CPU_PERIPHERAL_CTRL] &= ~(1<<0)

    def WREN(self):
        self.cmd(b'\x06')

    def WRDI(self):
        self.cmd(b'\x04')

    def WRSR(self, status):
        self.cmd(b'\x01', data=status)

    @property
    def RDSR(self):
        status = bytearray(1)
        self.cmd(b'\x05', rxbuf=status)
        return int.from_bytes(status, 1)

    @property
    def WIP(self):
        status = self.RDSR
        return status & (1<<0)

    @property
    def RDID(self):
        buffer = bytearray(3)
        self.cmd(b'\x9f', rxbuf=buffer)
        id = int.from_bytes(buffer, 3)
        manufactor_id = (id & 0xFF0000) >> 16
        device_id = id & 0xFFFF
        return (manufactor_id, device_id)

    def erasesector(self, addr):
        bytes_list = list(addr.to_bytes(3, 'little'))
        bytes_list.reverse()
        self.cmd(b'\x20', data = bytearray(bytes_list))

    def eraseblock(self, addr):
        bytes_list = list(addr.to_bytes(3, 'little'))
        bytes_list.reverse()
        self.cmd(b'\xd8', data = bytearray(bytes_list))

    def erasechip(self):
        self.cmd(b'\x60')

    def readblocks(self, n, buf):
        flash_cpu_ctrl_address = PERI_ON_BASE + REG_CPU_PERIPHERAL_CTRL
        ameba.mem32[flash_cpu_ctrl_address] |= (1<<0)
        dst_addr = uctypes.addressof(buf)
        src_addr = SPI_FLASH_BASE + (n + self.START_SEC) * self.SEC_SIZE
        fast_copy(dst_addr, src_addr, self.SEC_SIZE)
        ameba.mem32[flash_cpu_ctrl_address] &= ~(1<<0)

    def writeblocks(self, n, buf):
        address_base = SPI_FLASH_BASE + (n + self.START_SEC) * self.SEC_SIZE
        spi_status_address = SPI_FLASH_CTRL_BASE + REG_SPIC_SR
        flash_cpu_ctrl_address = PERI_ON_BASE + REG_CPU_PERIPHERAL_CTRL

        ameba.mem32[PERI_ON_BASE + REG_CPU_PERIPHERAL_CTRL] |= (1<<0)
        self.WREN()
        self.erasesector((n + self.START_SEC) * self.SEC_SIZE)
        while self.WIP != 0:
            pass

        arr = array.array("L", buf)

        for index, value in enumerate(arr):
            ameba.mem32[flash_cpu_ctrl_address] |= 0x01
            ameba.mem32[address_base + 4*index] = value

        ameba.mem32[PERI_ON_BASE + REG_CPU_PERIPHERAL_CTRL] &= ~(1<<0)

    def ioctl(self, op, arg):
        if op == 4: # BP_IOCTL_SEC_COUNT
            return self.blocks
        if op == 5: # BP_IOCTL_SEC_SIZE
            return self.SEC_SIZE
