include ../../py/mkenv.mk



# declare before py.mk
FROZEN_DIR = scripts
FROZEN_MPY_DIR = modules

include $(TOP)/py/py.mk

OS := $(shell uname)
TOOL := tool
CHIP := AMEBAD
DEBUG := 1
TC_VERSION := $(CHIP)
ifeq ($(CHIP), AMEBAD)
VENDOR := vendor/ambd_sdk
else
VENDOR := vendor
endif

$(info Now building for $(CHIP))
# toolchains version, note ameba1 uses V4.8.3, amebaD used ASDK V6.5.0, if unspecified,it uses V9.2.1
# CHIP would be AMEBA1, AMEBAD, AMEBAZ

# toolchains
ifeq ($(TC_VERSION), AMEBA1)  
TC_PATH := ../../../../toolchain/gcc-arm-none-eabi-4.8.3-2014q1/bin/
else ifeq ($(TC_VERSION), AMEBAD)
TC_PATH := ../../../../toolchain/asdk-6.5.0/bin/
else
TC_PATH :=
endif
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


ifeq ($(CHIP), AMEBA1)
include ameba1.mk
else ifeq ($(CHIP), AMEBAD)
include amebad.mk
else
include amebaz.mk
endif

include $(TOP)/py/mkrules.mk
