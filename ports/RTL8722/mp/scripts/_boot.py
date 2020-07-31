try:
    import uos
    import umachine
    import uterminal
    import flashbdev
    import uctypes
    import utime as time
except ImportError as e:
    print(e)

core = uos.uname().core
if core == 'RTL8195A':
    _baudrate = 38400
    _flash_vfs_start_addr = 0x100000
    _flash_vfs_block_num = 252
elif core == 'RTL8711BN':
    _baudrate = 115200
    _flash_vfs_start_addr = 0xA0000
    _flash_vfs_block_num = 95
else:
    pass

_loguart = umachine.LOGUART()
_loguart.init(baudrate=_baudrate)
uterminal.register(_loguart)
print("Init LOGUART %d finished and install it to uterminal list" % _baudrate)

if core == 'RTL8195A':
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
    
_flash = flashbdev.FlashBdev(start_addr=_flash_vfs_start_addr, blocks=_flash_vfs_block_num)
flash_vfs = uos.VfsFat(_flash)
print("Mounting Flashfs")
try:
    uos.mount(flash_vfs, '/pyboard')
except OSError as e:
    print("Mounting Flash to vfs at /flash failed, cause: {}".format(e))
    #print("Formating flash")
    #uos.VfsFat.mkfs(_flash)
    #print("Format done")
    #uos.mount(flash_vfs, '/flash')

try:
    uos.mount(flash_vfs, '/')
except OSError as e:
    print("Mounting Flash to vfs at / failed, cause: {}".format(e))

print("Mount flash to vfs done")

try:
    _ = uos.stat("/flash/main.py")
    uos.chdir("/flash")
    print("Executing main.py")
    execfile("main.py")
except OSError:
    print("main.py not found")
