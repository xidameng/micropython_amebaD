try:
    import uos
    import umachine
    import uterminal
    import uctypes
    import upip
except ImportError as e:
    print(e)


_baudrate = 38400
_log_uart = umachine.LOGUART()
_log_uart.init(baudrate=_baudrate)
uterminal.register(_log_uart)

print("Init LOGUART %d finished and install it to uterminal list" % _baudrate)

try:
    from rambdev import RamBdev
    print("found ramfs.py, trying to mount ramfs to fatfs ..")
    _ram = RamBdev(200) # 200 * 512 = 100K
    uos.VfsFat.mkfs(_ram)
    _ramvfs = uos.VfsFat(_ram)
    uos.mount(_ramvfs, '/ram')
except:
    pass


