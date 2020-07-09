ifeq ($(findstring CYGWIN, $(OS)), CYGWIN) 
PICK = $(BUILD)pick.exe
PAD  = $(BUILD)padding.exe
CHKSUM = $(BUILD)checksum.exe
POSTBUILD =$(BUILD)postbuild_img2_arduino_windows.exe
else
PICK = $(BUILD)/pick
PAD  = $(BUILD)/padding
AMEBASIZE = $(BUILD)/amebasize
POSTBUILD = $(BUILD)/postbuild_img2_arduino_linux
endif

################################
#        Include path          # 
################################
INC += -I.
INC += -Iinc/ameba
INC += -I$(TOP)
INC += -I$(BUILD)
INC += -I$(HEADER_BUILD)

INC += -I$(VENDOR)/project/realtek_amebaD_va0_example/inc/inc_hp
INC += -I$(VENDOR)/component/os/freertos
INC += -I$(VENDOR)/component/os/freertos/freertos_v10.2.0/Source/include
INC += -I$(VENDOR)/component/os/freertos/freertos_v10.2.0/Source/portable/GCC/ARM_CM33/non_secure
INC += -I$(VENDOR)/component/os/os_dep/include
INC += -I$(VENDOR)/component/soc/realtek/amebad/misc
INC += -I$(VENDOR)/component/common/api/network/include
INC += -I$(VENDOR)/component/common/api
INC += -I$(VENDOR)/component/common/api/at_cmd
INC += -I$(VENDOR)/component/common/api/platform
INC += -I$(VENDOR)/component/common/api/wifi
INC += -I$(VENDOR)/component/common/api/wifi/rtw_wpa_supplicant/src
INC += -I$(VENDOR)/component/common/api/wifi/rtw_wpa_supplicant/src/crypto
INC += -I$(VENDOR)/component/common/application
INC += -I$(VENDOR)/component/common/media/framework
INC += -I$(VENDOR)/component/common/example
INC += -I$(VENDOR)/component/common/example/wlan_fast_connect
INC += -I$(VENDOR)/component/common/mbed/api
INC += -I$(VENDOR)/component/common/mbed/hal
INC += -I$(VENDOR)/component/common/mbed/hal_ext
INC += -I$(VENDOR)/component/common/mbed/targets/hal/rtl8721d
INC += -I$(VENDOR)/component/common/network
INC += -I$(VENDOR)/component/common/network/lwip/lwip_v2.0.2/port/realtek/freertos
INC += -I$(VENDOR)/component/common/network/lwip/lwip_v2.0.2/src/include
INC += -I$(VENDOR)/component/common/network/lwip/lwip_v2.0.2/src/include/lwip
INC += -I$(VENDOR)/component/common/network/lwip/lwip_v2.0.2/port/realtek
INC += -I$(VENDOR)/component/common/test
INC += -I$(VENDOR)/component/soc/realtek/amebad/cmsis
INC += -I$(VENDOR)/component/soc/realtek/amebad/fwlib
INC += -I$(VENDOR)/component/soc/realtek/amebad/misc
INC += -I$(VENDOR)/component/common/drivers/wlan/realtek/include
INC += -I$(VENDOR)/component/common/drivers/wlan/realtek/src/osdep
INC += -I$(VENDOR)/component/common/drivers/wlan/realtek/src/hci
INC += -I$(VENDOR)/component/common/network/ssl/polarssl-1.3.8/include
INC += -I$(VENDOR)/component/common/network/ssl/ssl_ram_map/rom
INC += -I$(VENDOR)/component/common/utilities
INC += -I$(VENDOR)/component/common/video/v4l2/inc
INC += -I$(VENDOR)/component/common/media/rtp_codec
INC += -I$(VENDOR)/component/common/file_system/fatfs
INC += -I$(VENDOR)/component/common/file_system/fatfs/r0.10c/include
INC += -I$(VENDOR)/component/common/file_system/ftl
INC += -I$(VENDOR)/component/common/drivers/sdio/realtek/sdio_host/inc
INC += -I$(VENDOR)/component/common/audio
INC += -I$(VENDOR)/component/common/drivers/i2s
INC += -I$(VENDOR)/component/common/application/xmodem
INC += -I$(VENDOR)/component/common/network/mDNS
INC += -I$(VENDOR)/component/soc/realtek/amebad/fwlib/include
INC += -I$(VENDOR)/component/soc/realtek/amebad/swlib/string
INC += -I$(VENDOR)/component/soc/realtek/amebad/app/monitor/include
INC += -I$(VENDOR)/component/soc/realtek/amebad/app/xmodem
INC += -I$(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/include
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/board/amebad/lib
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/board/amebad/src
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/board/amebad/src/vendor_cmd
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/board/common/inc
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/example/ble_scatternet
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/inc/app
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/inc/bluetooth/gap
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/inc/bluetooth/profile
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/inc/bluetooth/profile/client
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/inc/bluetooth/profile/server
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/inc/os
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/inc/platform
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/inc/stack
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/src/mcu/module/data_uart_cmd
INC += -I$(VENDOR)/component/common/drivers/ir/protocol
INC += -I$(VENDOR)/component/common/drivers/wlan/realtek/src/core/option
INC += -I$(VENDOR)/component/common/drivers/wlan/realtek/src/hal
INC += -I$(VENDOR)/component/common/drivers/wlan/realtek/src/hal/phydm

INC += -I$(TOP)/extmod
INC += -I$(TOP)/lib/utils
INC += -I$(TOP)/lib/timeutils
INC += -I$(TOP)/lib/mp-readline
INC += -I$(TOP)/lib/netutils
INC += -I$(TOP)/lib/oofatfs

INC += -I./mphelper
INC += -I./mphelper/gccollect
INC += -I./mphelper/hal
INC += -I./mphelper/mods
INC += -I./mphelper/mods/network
INC += -I./mphelper/mods/machine



# Source file list
# -------------------------------------------------------------------
# micropython source
UPY_C += pins.c
UPY_C += mphelper/diskio.c
UPY_C += mphelper/exception.c
UPY_C += mphelper/help.c
UPY_C += mphelper/ameba/mphal.c
UPY_C += mphelper/input.c
UPY_C += mphelper/bufhelper.c
UPY_C += mphelper/mpthreadport.c
UPY_C += mphelper/gccollect/gccollect.c
UPY_C += mphelper/mods/modameba.c
UPY_C += mphelper/mods/modmachine.c
UPY_C += mphelper/mods/modlwip.c
UPY_C += mphelper/mods/moduwireless.c
UPY_C += mphelper/mods/modnetwork.c
UPY_C += mphelper/mods/modutime.c
UPY_C += mphelper/mods/modterm.c
UPY_C += mphelper/mods/moduos.c
UPY_C += mphelper/mods/modussl.c
UPY_C += mphelper/mods/machine/objloguart.c
UPY_C += mphelper/mods/machine/objwdt.c
UPY_C += mphelper/mods/machine/objflash.c
UPY_C += mphelper/mods/machine/objrtc.c
UPY_C += mphelper/mods/machine/objadc.c
UPY_C += mphelper/mods/machine/objdac.c
UPY_C += mphelper/mods/machine/objpin.c
UPY_C += mphelper/mods/machine/obji2c.c
UPY_C += mphelper/mods/machine/objpwm.c
UPY_C += mphelper/mods/machine/objtimer.c
UPY_C += mphelper/mods/machine/objspi.c
UPY_C += mphelper/mods/machine/objuart.c
UPY_C += mphelper/mods/machine/objcrypto.c
UPY_C += mphelper/mods/wireless/objwlan.c
UPY_C += mphelper/mods/network/objnetif.c
UPY_C += mphelper/mods/network/dhcps.c
UPY_C += \
		lib/utils/pyexec.c \
		lib/mp-readline/readline.c \
		lib/utils/interrupt_char.c \
		lib/timeutils/timeutils.c \
		lib/netutils/netutils.c \
		lib//utils/sys_stdio_mphal.c \
		lib/oofatfs/ff.c \
		lib/oofatfs/ffunicode.c \
		#lib/oofatfs/option/ccsbcs.c \

UPY_C += main.c


# Initialize target name and target object files
# -------------------------------------------------------------------

all: application manipulate_images

TARGET=application

# Include folder list
# -------------------------------------------------------------------

# Generate obj list
# -------------------------------------------------------------------

SRC_O = $(addprefix $(BUILD)/, $(UPY_C:.c=.o))

OBJ = $(PY_O) $(UPY_O)
SRC_QSTR += $(UPY_C)
SRC_QSTR_AUTO_DEPS +=

################################
# 			CFLAGS 			   #
################################

# Optimize level
CFLAGS = -O2

# CPU arch
CFLAGS += -march=armv8-m.main+dsp
CFLAGS += -mthumb 

# source code macro
CFLAGS += -ffunction-sections -mcmse -mfloat-abi=hard -mfpu=fpv5-sp-d16 -g -gdwarf-3 -nostartfiles -nodefaultlibs -nostdlib -O2 -D__FPU_PRESENT -gdwarf-3 -fstack-usage -fdata-sections -nostartfiles -nostdlib -Wall -Wpointer-arith -Wstrict-prototypes -Wundef -Wno-write-strings -Wno-maybe-uninitialized -c -MMD -Wextra
CFLAGS += $(INC)


###########################
#         LDFLAGS         #
###########################
LFLAGS =
LFLAGS += -O2 -march=armv8-m.main+dsp -mthumb -mcmse -mfloat-abi=hard -mfpu=fpv5-sp-d16 -nostartfiles -specs nosys.specs -Wl,--gc-sections
LFLAGS += -Wl,--cref -Wl,--build-id=none -Wl,-wrap,strcat -Wl,-wrap,strchr -Wl,-wrap,strcmp -Wl,-wrap,strncmp -Wl,-wrap,strcpy -Wl,-wrap,strncpy -Wl,-wrap,strlen -Wl,-wrap,strnlen -Wl,-wrap,strncat -Wl,-wrap,strpbrk -Wl,-wrap,strstr -Wl,-wrap,strtok -Wl,-wrap,strsep -Wl,-wrap,strtoll -Wl,-wrap,strtoul -Wl,-wrap,strtoull -Wl,-wrap,atoi -Wl,-wrap,malloc -Wl,-wrap,free -Wl,-wrap,realloc -Wl,-wrap,memcmp -Wl,-wrap,memcpy -Wl,-wrap,memmove -Wl,-wrap,memset -Wl,-wrap,printf -Wl,-wrap,sprintf -Wl,-wrap,snprintf -Wl,--no-enum-size-warning -Wl,--warn-common
LFLAGS += -Wl,-Map={build.path}/application.map 


###########################
#         LIBFLAGS        #
###########################
LIBAR += -L$(VENDOR)/ARCHIVE_LIB/ -l_arduino -l_wlan -l_wps -l_wlan_mp -l_wifi_ucps_fw -l_wifi_fw -l_websocket -l_user -l_usbh -l_usbd -l_tftp -l_mdns -l_m4a_self -l_httpd -l_httpc -l_http2 -l_eap -l_dct -l_coap -l_cmsis_dsp -l_bt 
LIBFLAGS += -Wl,--cref -Wl,--build-id=none -Wl,-wrap,strcat -Wl,-wrap,strchr -Wl,-wrap,strcmp -Wl,-wrap,strncmp -Wl,-wrap,strcpy 

###########################
#         BUILD RULES     #
###########################
application: prerequirement build_info $(PY_O)
	$(ECHO) "Building $(CHIP) "
	$(Q)$(LD) $(LFLAGS) -o $(BUILD)/$(TARGET).axf $(OBJ) -T$(TOOL)/amebad/rlx8721d_img2_is_arduino.ld -Wl,-Map=$(BUILD)/$(TARGET).map $(LIBFLAGS) \
	-Wl,--start-group $(PY_O)-Wl,--end-group -Wl,--start-group -Wl,--no-whole-archive $(LIBAR) -Wl,--end-group -lm -lstdc++
	$(Q)$(OBJDUMP) -d $(BUILD)/$(TARGET).axf > $(BUILD)/$(TARGET).asm

.PHONY: manipulate_images
manipulate_images: $(PICK) $(PAD) $(AMEBASIZE) $(POSTBUILD)
	$(Q)echo ===========================================================
	$(Q)echo Image manipulating
	$(Q)echo ===========================================================
	
	$(Q)$(STRIP) $(BUILD)/$(TARGET).axf
	$(Q)$(POSTBUILD) $(BUILD) $(BUILD)/$(TARGET).axf $(TC_PATH)

	$(Q)echo ===========================
	$(Q)echo  End of Image manipulating
	$(Q)echo ===========================

# Generate build info
# -------------------------------------------------------------------	

.PHONY: build_info
build_info:
	@echo \#define UTS_VERSION \"`date +%Y/%m/%d-%T`\" > .ver
	@echo \#define AMEBA_COMPILE_TIME \"`date +%Y/%m/%d-%T`\" >> .ver
	@echo \#define AMEBA_COMPILE_DATE \"`date +%Y%m%d`\" >> .ver
	@echo \#define AMEBA_COMPILE_BY \"`id -u -n`\" >> .ver
	@echo \#define AMEBA_COMPILE_HOST \"`$(HOSTNAME_APP)`\" >> .ver
	@if [ -x /bin/dnsdomainname ]; then \
		echo \#define AMEBA_COMPILE_DOMAIN \"`dnsdomainname`\"; \
	elif [ -x /bin/domainname ]; then \
		echo \#define AMEBA_COMPILE_DOMAIN \"`domainname`\"; \
	else \
		echo \#define AMEBA_COMPILE_DOMAIN ; \
	fi >> .ver

	@echo \#define AMEBA_COMPILER \"gcc `$(CC) $(CFLAGS) -dumpversion `\" >> .ver
	@mv -f .ver inc/ameba/$@.h


.PHONY: prerequirement
prerequirement:
	$(Q)echo ===========================================================
	$(Q)echo Build $(TARGET)
	$(Q)echo ===========================================================
	$(Q)$(MKDIR) -p $(BUILD)
	$(Q)cp -f $(TOOL)/amebad/image/km0_boot_all.bin $(BUILD)/km0_boot_all.bin
	$(Q)chmod +rw $(BUILD)/km0_boot_all.bin
	$(Q)cp -f $(TOOL)/amebad/image/km0_image2_all.bin $(BUILD)/km0_image2_all.bin
	$(Q)chmod +rw $(BUILD)/km0_image2_all.bin
	$(Q)cp -f $(TOOL)/amebad/image/km4_boot_all.bin $(BUILD)/km4_boot_all.bin
	$(Q)chmod +rw $(BUILD)/km4_boot_all.bin

##############
#    Tools   #
##############
$(PICK):
	$(Q)cp -f $(TOOL)/pick $(BUILD)/pick

$(PAD):
	$(Q)cp -f $(TOOL)/PAD $(BUILD)/PAD

$(AMEBASIZE):
	$(Q)cp -f $(TOOL)/AMEBASIZE $(BUILD)/AMEBASIZE

$(postbuild):
	$(Q)cp -f $(TOOL)/postbuild_img2_arduino_linux $(BUILD)/postbuild_img2_arduino_linux

$(UPY_O): $(BUILD)/%.o : %.c
	$(Q)$(MKDIR) -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: flashburn
flashburn:
	$(Q)$(CP) -f $(TOOL)/ameba1/target_NORMALB.axf $(BUILD)/target_NORMAL.axf
	$(Q)$(CP) -f $(TOOL)/ameba1/SetupGDB_NORMAL.sh $(BUILD)/SetupGDB_NORMAL.sh
	$(Q)$(CP) -f $(TOOL)/ameba1/rtl_gdb_flash_write_openocd.txt $(BUILD)/rtl_gdb_flash_write_openocd.txt
	$(Q)chmod +rw $(BUILD)/target_NORMAL.axf
	$(Q)chmod +rx $(BUILD)/SetupGDB_NORMAL.sh
	$(Q)$(BUILD)/SetupGDB_NORMAL.sh
	$(Q)$(GDB) -x $(BUILD)/rtl_gdb_flash_write_openocd.txt
	
.PHONY: ramdebug
ramdebug:
	$(Q)$(GDB) -x $(TOOL)/ameba1/rtl_gdb_ramdebug_openocd.txt	