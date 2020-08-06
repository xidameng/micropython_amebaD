import Pin
c = Pin("PA_19", dir=Pin.OUT)
c.value(1)
c.value(0)
c.value(1)
c.value(0)
c.value(1)
c.value(0)
c.value(1)
c.value(0)
c.value(1)
c.value(0)
c.value(1)
c.value(0)
c.value(1)
c.value(0)
c.value(1)
c.value(0)
c.value(1)
c.value(0)
c.value(1)
c.value(0)
c.value(1)
c.value(0)
c.value(1)
c.value(0)


from umachine import Pin 

a = Pin.("PA_19", dir=Pin.OUT)
a.value(1)
a.value(0)
a.value(1)
a.value(0)
a.value(1)
a.value(0)
a.value(1)
a.value(0)
a.value(1)
a.value(0)
a.value(1)
a.value(0)
a.value(1)
a.value(0)
a.value(1)
a.value(0)
a.value(1)
a.value(0)
a.value(1)
a.value(0)
a.value(1)
a.value(0)
a.value(1)
a.value(0)


from hardware import Pin
b = Pin("PA_19", dir=Pin.OUT)
b.toggle()
b.value(1)
b.value(0)



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
