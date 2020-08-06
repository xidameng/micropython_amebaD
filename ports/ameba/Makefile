#include ../../py/mkenv.mk


# declare before py.mk
FROZEN_DIR = scripts
FROZEN_MPY_DIR = modules

#include $(TOP)/py/py.mk

OS := $(shell uname)
CHIP := AMEBAD
DEBUG := 1

ifeq ($(CHIP), AMEBAD)
VENDOR := vendor/sdk
TOOL := tool/amebad
else ifeq ($(CHIP), AMEBA1)
VENDOR := vendor
TOOL := tool/ameba1
else
VENDOR := vendor
TOOL := tool/amebaz
endif

$(info Desired building environment: Ubuntu 20)
$(info Current building target: $(CHIP))
# Note that ameba1 uses toolchain V4.8.3, amebaD used ASDK V6.5.0, if unspecified,it uses the one installed under usr/bin
# CHIP would be AMEBA1, AMEBAD or AMEBAZ

# toolchains
ifeq ($(CHIP), AMEBA1)  
TC_PATH := tool/toolchain/gcc-arm-none-eabi-4.8.3-2014q1/bin/
else ifeq ($(CHIP), AMEBAD)
TC_PATH := tool/toolchain/asdk-6.5.0/bin/
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
POSTBUILD = $(BUILD)/postbuild_img2_mp_linux
IMAGETOOL = amebad_image_tool

ifeq ($(CHIP), AMEBA1)
include ameba1.mk
else ifeq ($(CHIP), AMEBAD)
include amebad.mk
else
include amebaz.mk
endif

#include $(TOP)/py/mkrules.mk
