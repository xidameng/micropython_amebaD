try:
    import uos
    import umachine
    import uterminal
    import flashbdev
    import uctypes
except ImportError as e:
    print(e)
_baudrate = 38400
_log_uart = umachine.LOGUART()
_log_uart.init(baudrate=_baudrate)
#_log_uart.irq_enable(True)
#_log_uart.irq_handler(uterminal.rx_post)
#uterminal.dump().append(_log_uart)
uterminal.register(_log_uart)

#uart = umachine.UART(0, tx="PA_7", rx="PA_6", baudrate=9600)
#uart.init()
#uterminal.register(uart)

print("Init LOGUART %d finished and install it to uterminal list" % _baudrate)
try:
    import rambdev
    print("found ramfs.py, try to mount ram to vfs")
    ram = rambdev.RamBdev(200)
    uos.VfsFat.mkfs(ram)
    ramvfs = uos.VfsFat(ram)
    uos.mount(ramvfs, '/ram')
except:
    pass
_flash = flashbdev.FlashBdev()
try:
    flash_vfs = uos.VfsFat(_flash)
    print("mounting flash to vfs ...")
    uos.mount(flash_vfs, '/flash')
    uos.chdir('/flash')
except:
    print("mount flash to vfs failed, formating flash ... ")
    uos.VfsFat.mkfs(_flash)
    flash_vfs = uos.VfsFat(_flash)
    print("mounting flash to vfs ...")
    uos.mount(flash_vfs, '/flash')
    uos.chdir('/flash')
