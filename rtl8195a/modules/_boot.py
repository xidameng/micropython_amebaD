try:
    import uos
    import umachine
except ImportError as e:
    print(e)

flash = umachine.FLASH()

try:
    flash_vfs = uos.VfsFat(flash, "/flash")
except OSError:
    print("mount flash to file system failed, formating flash ...")
    uos.VfsFat.mkfs(flash)
    flash_vfs = uos.VfsFat(flash, "/flash")

print("mount flash to file system done")
