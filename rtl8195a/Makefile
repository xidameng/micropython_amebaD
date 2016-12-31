include ../py/mkenv.mk

QSTR_DEFS += qstrdefsport.h

# declare before py.mk
MICROPY_PY_LWIP := 1
FROZEN_DIR = scripts
FROZEN_MPY_DIR = modules

include ../py/py.mk

OS := $(shell uname)
VENDOR := vendor
GDB_SERVER := openocd
TOOL := tool
CHIP := MP_RTL8195A
# CHIP would be MP_RTL8195A, MP_RTL8711AM, MP_RTL8711AF, MP_RTL8710AF

# -------------------------------------------------------------------
AMEBA_TOOLDIR = $(VENDOR)/component/soc/realtek/8195a/misc/iar_utility/common/tools/
FLASH_TOOLDIR = $(VENDOR)/component/soc/realtek/8195a/misc/gcc_utility/

CROSS_COMPILE = arm-none-eabi-

# Compilation tools
AR = $(CROSS_COMPILE)ar
CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
NM = $(CROSS_COMPILE)nm
LD = $(CROSS_COMPILE)gcc
GDB = $(CROSS_COMPILE)gdb
SIZE = $(CROSS_COMPILE)size
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

ifeq ($(findstring CYGWIN, $(OS)), CYGWIN) 
PICK = $(AMEBA_TOOLDIR)pick.exe
PAD  = $(AMEBA_TOOLDIR)padding.exe
CHKSUM = $(AMEBA_TOOLDIR)checksum.exe
else
PICK = $(AMEBA_TOOLDIR)pick
PAD  = $(AMEBA_TOOLDIR)padding
CHKSUM = $(AMEBA_TOOLDIR)checksum
endif


# Initialize target name and target object files
# -------------------------------------------------------------------

all: application manipulate_images

mp: application manipulate_images

TARGET=application

# Include folder list
# -------------------------------------------------------------------

include $(TARGET).mk

# Generate obj list
# -------------------------------------------------------------------

SRC_O = $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
DRAM_O = $(addprefix $(BUILD)/, $(DRAM_C:.c=.o))
ASM_O = $(addprefix $(BUILD)/, $(ASM_S:.s=.o))

OBJ = $(PY_O) $(SRC_O) $(DRAM_O) $(ASM_O)

SRC_QSTR += $(SRC_C) $(DRAM_C)
SRC_QSTR_AUTO_DEPS +=

# Compile options
# -------------------------------------------------------------------

CFLAGS =
CFLAGS += -DM3 -DCONFIG_PLATFORM_8195A -DGCC_ARMCM3 -DARDUINO_SDK -D$(CHIP)
CFLAGS += -mcpu=cortex-m3 -mthumb -g2 -w -O2 -Wno-pointer-sign -fno-common -fmessage-length=0  -ffunction-sections -fdata-sections -fomit-frame-pointer -fno-short-enums -mcpu=cortex-m3 -DF_CPU=166000000L -std=gnu99 -fsigned-char
CFLAGS += $(INC)

LFLAGS = 
LFLAGS += -mcpu=cortex-m3 -mthumb -g --specs=nano.specs -nostartfiles -Wl,-Map=$(BUILD)/application.map -Os -Wl,--gc-sections -Wl,--cref -Wl,--entry=Reset_Handler -Wl,--no-enum-size-warning -Wl,--no-wchar-size-warning

LIBFLAGS =
all: LIBFLAGS += -L$(VENDOR)/component/soc/realtek/8195a/misc/bsp/lib/common/GCC/ -l_platform -l_wlan -l_p2p -l_wps -l_rtlstd -l_websocket -l_xmodem -l_sdcard -lm -lc -lnosys -lgcc
mp: LIBFLAGS += -L$(VENDOR)/component/soc/realtek/8195a/misc/bsp/lib/common/GCC/ -l_platform -l_wlan_mp -l_p2p -l_wps -l_rtlstd -l_websocket -l_xmodem -l_sdcard -lm -lc -lnosys -lgcc

RAMALL_BIN =
OTA_BIN = 
all: RAMALL_BIN = ram_all.bin
all: OTA_BIN = ota.bin
mp: RAMALL_BIN = ram_all_mp.bin
mp: OTA_BIN = ota_mp.bin

application: prerequirement build_info $(SRC_O) $(DRAM_O) $(ASM_O) $(PY_O)
	$(ECHO) "Building $(CHIP) "
	$(Q)$(LD) $(LFLAGS) -o $(BUILD)/$(TARGET).axf $(OBJ) $(BUILD)/ram_1.r.o $(LIBFLAGS) -L$(TOOL) -l_analout_api -l_usbd -l_usbh -T$(TOOL)/rlx8195A-symbol-v02-img2.ld 
	$(Q)$(OBJDUMP) -d $(BUILD)/$(TARGET).axf > $(BUILD)/$(TARGET).asm

.PHONY: add_prefix
add_prefix:
	$(Q)$(foreach OBJ, $(PY_O), $(OBJCOPY) --prefix-alloc-sections .sdram $(OBJ);)
	

.PHONY: manipulate_images
manipulate_images:	
	$(Q)echo ===========================================================
	$(Q)echo Image manipulating
	$(Q)echo ===========================================================
	$(Q)$(NM) $(BUILD)/$(TARGET).axf | sort > $(BUILD)/$(TARGET).nmap
	$(Q)$(OBJCOPY) -j .image2.start.table -j .ram_image2.text -j .ram_image2.rodata -j .ram.data -Obinary $(BUILD)/$(TARGET).axf $(BUILD)/ram_2.bin
	$(Q)$(OBJCOPY) -j .sdr_text -j .sdr_rodata -j .sdr_data -Obinary $(BUILD)/$(TARGET).axf $(BUILD)/sdram.bin
	$(Q)cp $(VENDOR)/component/soc/realtek/8195a/misc/bsp/image/ram_1.p.bin $(BUILD)/ram_1.p.bin
	$(Q)chmod +rw $(BUILD)/ram_1.p.bin
	$(Q)chmod +rx $(PICK) $(CHKSUM) $(PAD)
	$(Q)$(PICK) 0x`grep __ram_image2_text_start__ $(BUILD)/$(TARGET).nmap | gawk '{print $$1}'` 0x`grep __ram_image2_text_end__ $(BUILD)/$(TARGET).nmap | gawk '{print $$1}'` $(BUILD)/ram_2.bin $(BUILD)/ram_2.p.bin body+reset_offset+sig
	$(Q)$(PICK) 0x`grep __ram_image2_text_start__ $(BUILD)/$(TARGET).nmap | gawk '{print $$1}'` 0x`grep __ram_image2_text_end__ $(BUILD)/$(TARGET).nmap | gawk '{print $$1}'` $(BUILD)/ram_2.bin $(BUILD)/ram_2.ns.bin body+reset_offset
	$(Q)$(PICK) 0x`grep __sdram_data_start__ $(BUILD)/$(TARGET).nmap | gawk '{print $$1}'` 0x`grep __sdram_data_end__ $(BUILD)/$(TARGET).nmap | gawk '{print $$1}'` $(BUILD)/sdram.bin $(BUILD)/ram_3.p.bin body+reset_offset
	$(Q)$(PAD) 44k 0xFF $(BUILD)/ram_1.p.bin
	$(Q)cat $(BUILD)/ram_1.p.bin > $(BUILD)/$(RAMALL_BIN)
	$(Q)chmod 777 $(BUILD)/$(RAMALL_BIN)
	$(Q)cat $(BUILD)/ram_2.p.bin >> $(BUILD)/$(RAMALL_BIN)
	$(Q)if [ -s $(BUILD)/sdram.bin ]; then cat $(BUILD)/ram_3.p.bin >> $(BUILD)/$(RAMALL_BIN); fi
	$(Q)cat $(BUILD)/ram_2.ns.bin > $(BUILD)/$(OTA_BIN)
	$(Q)chmod 777 $(BUILD)/$(OTA_BIN)
	$(Q)if [ -s $(BUILD)/sdram.bin ]; then cat $(BUILD)/ram_3.p.bin >> $(BUILD)/$(OTA_BIN); fi
	$(Q)$(CHKSUM) $(BUILD)/$(OTA_BIN) || true
	$(Q)$(RM) $(BUILD)/ram_*.p.bin $(BUILD)/ram_*.ns.bin
	$(Q)$(SIZE) -d $(BUILD)/$(TARGET).axf
	$(Q)du -h $(BUILD)/$(RAMALL_BIN)

# Generate build info
# -------------------------------------------------------------------	

.PHONY: build_info
build_info:
	@echo \#define UTS_VERSION \"`date +%Y/%m/%d-%T`\" > .ver
	@echo \#define RTL8195AFW_COMPILE_TIME \"`date +%Y/%m/%d-%T`\" >> .ver
	@echo \#define RTL8195AFW_COMPILE_DATE \"`date +%Y%m%d`\" >> .ver
	@echo \#define RTL8195AFW_COMPILE_BY \"`id -u -n`\" >> .ver
	@echo \#define RTL8195AFW_COMPILE_HOST \"`$(HOSTNAME_APP)`\" >> .ver
	@if [ -x /bin/dnsdomainname ]; then \
		echo \#define RTL8195AFW_COMPILE_DOMAIN \"`dnsdomainname`\"; \
	elif [ -x /bin/domainname ]; then \
		echo \#define RTL8195AFW_COMPILE_DOMAIN \"`domainname`\"; \
	else \
		echo \#define RTL8195AFW_COMPILE_DOMAIN ; \
	fi >> .ver

	@echo \#define RTL195AFW_COMPILER \"gcc `$(CC) $(CFLAGS) -dumpversion | tr --delete '\r'`\" >> .ver
	@mv -f .ver inc/$@.h


.PHONY: prerequirement
prerequirement:
	$(Q)echo ===========================================================
	$(Q)echo Build $(TARGET)
	$(Q)echo ===========================================================
	$(Q)$(MKDIR) -p $(BUILD)
	$(Q)$(MKDIR) -p $(BUILD)
	$(Q)cp $(VENDOR)/component/soc/realtek/8195a/misc/bsp/image/ram_1.r.bin $(BUILD)/ram_1.r.bin
	$(Q)chmod +rw $(BUILD)/ram_1.r.bin
	$(Q)$(OBJCOPY) --rename-section .data=.loader.data,contents,alloc,load,readonly,data -I binary -O elf32-littlearm -B arm $(BUILD)/ram_1.r.bin $(BUILD)/ram_1.r.o 

$(SRC_O): $(BUILD)/%.o : %.c
	$(Q)$(MKDIR) -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
	$(Q)$(CC) $(CFLAGS) -c $< -MM -MT $@ -MF $(BUILD)/$(notdir $(patsubst %.o,%.d,$@))

$(DRAM_O): $(BUILD)/%.o : %.c 
	$(Q)$(MKDIR) -p $(dir $@)
	$(CC) $(CFLAGS)  -c $< -o $@
	$(Q)$(OBJCOPY) --prefix-alloc-sections .sdram $@
	$(Q)$(CC) $(CFLAGS)  -c $< -MM -MT $@ -MF $(BUILD)/$(notdir $(patsubst %.o,%.d,$@))

$(ASM_O): $(BUILD)/%.o : %.s
	$(Q)$(MKDIR) -p $(dir $@)
	$(CC) $(CFLAGS)  -c $< -o $@
	$(Q)$(OBJCOPY) --prefix-alloc-sections .sdram $@
	$(Q)$(CC) $(CFLAGS)  -c $< -MM -MT $@ -MF $(BUILD)/$(notdir $(patsubst %.o,%.d,$@))

.PHONY: setup
setup:
	@echo "----------------"
	@echo Setup $(GDB_SERVER)
	@echo "----------------"
ifeq ($(GDB_SERVER), openocd)
	$(Q)cp -p $(FLASH_TOOLDIR)/rtl_gdb_debug_openocd.txt $(FLASH_TOOLDIR)/rtl_gdb_debug.txt
	$(Q)cp -p $(FLASH_TOOLDIR)/rtl_gdb_ramdebug_openocd.txt $(FLASH_TOOLDIR)/rtl_gdb_ramdebug.txt
	$(Q)cp -p $(FLASH_TOOLDIR)/rtl_gdb_flash_write_openocd.txt $(FLASH_TOOLDIR)/rtl_gdb_flash_write.txt
else
	$(Q)cp -p $(FLASH_TOOLDIR)/rtl_gdb_debug_jlink.txt $(FLASH_TOOLDIR)/rtl_gdb_debug.txt
	$(Q)cp -p $(FLASH_TOOLDIR)/rtl_gdb_ramdebug_jlink.txt $(FLASH_TOOLDIR)/rtl_gdb_ramdebug.txt
	$(Q)cp -p $(FLASH_TOOLDIR)/rtl_gdb_flash_write_jlink.txt $(FLASH_TOOLDIR)/rtl_gdb_flash_write.txt
endif

.PHONY: flashburn
flashburn:
	$(Q)@if [ ! -f $(FLASH_TOOLDIR)/rtl_gdb_flash_write.txt ] ; then echo Please do \"make setup GDB_SERVER=[jlink or openocd]\" first; echo && false ; fi
ifeq ($(findstring CYGWIN, $(OS)), CYGWIN) 
	$(Q)$(FLASH_TOOLDIR)/Check_Jtag.sh
endif
	$(Q)$(CP)	$(FLASH_TOOLDIR)/target_NORMALB.axf $(FLASH_TOOLDIR)/target_NORMAL.axf
	$(Q)chmod +rw $(FLASH_TOOLDIR)/target_NORMAL.axf
	$(Q)chmod +rx $(FLASH_TOOLDIR)/SetupGDB_NORMAL.sh
	$(Q)$(FLASH_TOOLDIR)/SetupGDB_NORMAL.sh
	$(Q)$(GDB) -x $(FLASH_TOOLDIR)/rtl_gdb_flash_write.txt
	
.PHONY: debug
debug:
	$(Q)@if [ ! -f $(FLASH_TOOLDIR)/rtl_gdb_debug.txt ] ; then echo Please do \"make setup GDB_SERVER=[jlink or openocd]\" first; echo && false ; fi
ifeq ($(findstring CYGWIN, $(OS)), CYGWIN) 
	$(Q)$(FLASH_TOOLDIR)/Check_Jtag.sh
	$(Q)cmd /c start $(GDB) -x $(FLASH_TOOLDIR)/rtl_gdb_debug.txt
else
	$(Q)$(GDB) -x $(FLASH_TOOLDIR)/rtl_gdb_debug.txt
endif

.PHONY: ramdebug
ramdebug:
	$(Q)@if [ ! -f $(FLASH_TOOLDIR)/rtl_gdb_ramdebug.txt ] ; then echo Please do \"make setup GDB_SERVER=[jlink or openocd]\" first; echo && false ; fi
ifeq ($(findstring CYGWIN, $(OS)), CYGWIN) 
	$(Q)$(FLASH_TOOLDIR)/Check_Jtag.sh
	$(Q)cmd /c start $(GDB) -x $(FLASH_TOOLDIR)/rtl_gdb_ramdebug.txt	
else
	$(Q)$(GDB) -x $(FLASH_TOOLDIR)/rtl_gdb_ramdebug.txt	
endif

.PHONY: print
print:
	echo $(SRC_C)

include ../py/mkrules.mk
