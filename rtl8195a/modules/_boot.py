try:
    import uos
    import umachine
except ImportError as e:
    print(e)

flash = umachine.FLASH()

try:
    flash_vfs = uos.VfsFat(flash, "")
except OSError:
    uos.VfsFat.mkfs(flash)
    flash_vfs = uos.VfsFat(flash, "")

print("mount flash to file system done")
