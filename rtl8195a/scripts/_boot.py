try:
    import uos
    import umachine
    import uterminal
    import flashbdev
    import uctypes
except ImportError as e:
    print(e)
_baudrate = 115200
_log_uart = umachine.LOGUART()
_log_uart.init(baudrate=_baudrate)
uterminal.register(_log_uart)
print("Init LOGUART %d finished and install it to uterminal list" % _baudrate)
try:
    ram_block_numbers = 200 # 512 * 200 = 100k
    import rambdev
    print("Mounting Ramfs, ramfs size = {}".format(ram_block_numbers * 512))
    ram = rambdev.RamBdev(ram_block_numbers)
    uos.VfsFat.mkfs(ram)
    ramvfs = uos.VfsFat(ram)
    uos.mount(ramvfs, '/ram')
except Exception as e:
    print("Mounting Ramfs failed, cause {}".format(e))

_flash = flashbdev.FlashBdev()
flash_vfs = uos.VfsFat(_flash)
print("Mounting Flashfs")
try:
    uos.mount(flash_vfs, '/flash')
except OSError as e:
    print("Mounting Flash to vfs failed, cause: {}".format(e))
    print("Formating flash")
    uos.VfsFat.mkfs(_flash)
    print("Format done")
    uos.mount(flash_vfs, '/flash')

print("Mount flash to vfs done")
uos.chdir("/flash")

try:
    _ = uos.stat("/flash/main.py")
    print("Executing main.py")
    execfile("main.py")
except OSError:
    print("main.py not found")
