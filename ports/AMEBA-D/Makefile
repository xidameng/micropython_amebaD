include ../../py/mkenv.mk


# declare before py.mk
FROZEN_DIR = scripts
FROZEN_MPY_DIR = modules

include $(TOP)/py/py.mk

OS := $(shell uname)
CHIP := AMEBAD
DEBUG := 1
VENDOR := vendor/sdk
TOOL := tool/amebad

$(info Note: Only support building on Cygwin and Linux!)
$(info Now building for: $(CHIP))


# Declaring tools depending on OS
ifeq ($(findstring CYGWIN, $(OS)), CYGWIN) 
TC_PATH := tool/toolchain/cygwin/ameba_d_toolchain_windows/bin/
CROSS_COMPILE := arm-none-eabi-
AR = $(TC_PATH)$(CROSS_COMPILE)ar.exe
CC = $(TC_PATH)$(CROSS_COMPILE)gcc.exe
AS = $(TC_PATH)$(CROSS_COMPILE)as.exe
NM = $(TC_PATH)$(CROSS_COMPILE)nm.exe
LD = $(TC_PATH)$(CROSS_COMPILE)gcc.exe
GDB = $(TC_PATH)$(CROSS_COMPILE)gdb.exe
SIZE = $(TC_PATH)$(CROSS_COMPILE)size.exe
OBJCOPY = $(TC_PATH)$(CROSS_COMPILE)objcopy.exe
OBJDUMP = $(TC_PATH)$(CROSS_COMPILE)objdump.exe
STRIP = $(TC_PATH)$(CROSS_COMPILE)strip.exe
POSTBUILD = postbuild_img2_arduino_windows.exe
IMAGETOOL = amebad_image_tool.exe
PICK = pick.exe
PAD  = pad.exe
BUILDTOOL_PATH := tools/windows
UPLOAD_PATH := COM8
POSTBUILDTOOL_PATH := tool/amebad/windows
MPY_CROSS = $(TOP)/mpy-cross/mpy-cross.exe
else
TC_PATH := tool/toolchain/linux/asdk-6.5.0/bin/
CROSS_COMPILE := arm-none-eabi-
AR = $(TC_PATH)$(CROSS_COMPILE)ar
CC = $(TC_PATH)$(CROSS_COMPILE)gcc
AS = $(TC_PATH)$(CROSS_COMPILE)as
NM = $(TC_PATH)$(CROSS_COMPILE)nm
LD = $(TC_PATH)$(CROSS_COMPILE)gcc
GDB = $(TC_PATH)$(CROSS_COMPILE)gdb
SIZE = $(TC_PATH)$(CROSS_COMPILE)size
OBJCOPY = $(TC_PATH)$(CROSS_COMPILE)objcopy
OBJDUMP = $(TC_PATH)$(CROSS_COMPILE)objdump
STRIP = $(TC_PATH)$(CROSS_COMPILE)strip
POSTBUILD = postbuild_img2_arduino_linux
IMAGETOOL = amebad_image_tool
PICK = pick
PAD  = pad
BUILDTOOL_PATH := tools/linux
UPLOAD_PATH := /dev/ttyUSB0
POSTBUILDTOOL_PATH := tool/amebad/linux
endif

include amebad.mk

include $(TOP)/py/mkrules.mk
