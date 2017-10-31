#!/bin/sh

#===============================================================================
CURRENT_UTILITY_DIR=$(pwd)
echo "..."
echo $CURRENT_UTILITY_DIR
BOOTALLFILENAME="./build/boot_all.bin"
RAMFILENAME="./build/image2_all_ota1.bin"

echo $RAMFILENAME

#===============================================================================
#get file size
BOOTALL_FILE_SIZE=$(stat -c %s $BOOTALLFILENAME)
BOOTALL_FILE_SIZE_HEX=`echo "obase=16; $BOOTALL_FILE_SIZE"|bc`

RAM_FILE_SIZE=$(stat -c %s $RAMFILENAME)
RAM_FILE_SIZE_HEX=`echo "obase=16; $RAM_FILE_SIZE"|bc`

echo "size "$BOOTALL_FILE_SIZE" --> 0x"$BOOTALL_FILE_SIZE_HEX
echo "size "$RAM_FILE_SIZE" --> 0x"$RAM_FILE_SIZE_HEX

echo "set \$BOOTALLFILESize = 0x$BOOTALL_FILE_SIZE_HEX" > BTAsize.gdb
echo "set \$RamFileSize = 0x$RAM_FILE_SIZE_HEX" > fwsize.gdb
exit
