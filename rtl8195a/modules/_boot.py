try:
    import uos
    import umachine
    import uterminal
except ImportError as e:
    print(e)
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
