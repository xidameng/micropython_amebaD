include ../../py/mkenv.mk

# declare before py.mk
FROZEN_DIR = scripts
FROZEN_MPY_DIR = modules

include $(TOP)/py/py.mk

OS := $(shell uname)
VENDOR := vendor
TOOL := tool
CHIP := AMEBA1
DEBUG := 1
# CHIP would be AMEBA1, AMEBAZ

# toolchains
CROSS_COMPILE := arm-none-eabi-
AR = $(CROSS_COMPILE)ar
CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
NM = $(CROSS_COMPILE)nm
LD = $(CROSS_COMPILE)gcc
GDB = $(CROSS_COMPILE)gdb
SIZE = $(CROSS_COMPILE)size
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

ifeq ($(CHIP), AMEBA1)
include ameba1.mk
else
include amebaz.mk
endif

include $(TOP)/py/mkrules.mk
