include ../../py/mkenv.mk

# declare before py.mk
FROZEN_DIR = mp/scripts
FROZEN_MPY_DIR = mp/modules

include $(TOP)/py/py.mk

OS := $(shell uname)
CHIP := AMEBAD
DEBUG := 1
VENDOR := inc/sdk
TOOL := postbuildTool
LD_PATH = ld_lib

$(info Now building for $(CHIP))

# toolchains
TC_PATH := toolchain/asdk-6.5.0/bin/
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

include amebad.mk

include $(TOP)/py/mkrules.mk