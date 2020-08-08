OS := $(shell uname)
CHIP := AMEBAD
DEBUG := 1
VENDOR := inc/sdk
TOOL := postbuildTool
LD_PATH = ld_lib
Q := @
BUILD:= build

$(info Now building for $(CHIP))

# Declaring tools depending on OS
ifeq ($(findstring CYGWIN, $(OS)), CYGWIN) 
TC_PATH := toolchain/ameba_d_asdk_toolchain/1.0.1/bin/
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
else
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
POSTBUILD = postbuild_img2_arduino_linux
IMAGETOOL = amebad_image_tool
PICK = pick
PAD  = pad
BUILDTOOL_PATH := tools/linux
endif

#include ../../py/mkenv.mk

# declare before py.mk
#FROZEN_DIR = mp/scripts
#FROZEN_MPY_DIR = mp/modules

#include $(TOP)/py/py.mk


################################
#        Include path          # 
################################
INC = -I.
INC += -I$(VENDOR)/project/{ameba.project}/inc/inc_hp
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
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/example/bt_config
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/inc/app
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/inc/bluetooth/gap
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/inc/bluetooth/profile
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/inc/bluetooth/profile/client
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/inc/bluetooth/profile/server
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/inc/os
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/inc/platform
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/inc/stack
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/src/app/ble_central_client
INC += -I$(VENDOR)/component/common/bluetooth/realtek/sdk/src/mcu/module/data_uart_cmd
INC += -I$(VENDOR)/component/common/drivers/ir/protocol
INC += -I$(VENDOR)/component/common/drivers/wlan/realtek/src/core/option
INC += -I$(VENDOR)/component/common/drivers/wlan/realtek/src/hal
INC += -I$(VENDOR)/component/common/drivers/wlan/realtek/src/hal/phydm
INC += -I$(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/include


UPY_C = main.c


# Initialize target name and target object files
# -------------------------------------------------------------------

all: application manipulate_images

TARGET=application

# Generate obj list
# -------------------------------------------------------------------

UPY_O = $(addprefix $(BUILD)/, $(UPY_C:.c=.o))
#$(PY_O)
OBJ = $(UPY_O)
SRC_QSTR += $(UPY_C)
SRC_QSTR_AUTO_DEPS +=

################################
# 			CFLAGS 			   #
################################

CFLAGS = -ffunction-sections -march=armv8-m.main+dsp -mthumb -mcmse -mfloat-abi=hard -mfpu=fpv5-sp-d16 -g -gdwarf-3 -nostartfiles -nodefaultlibs -nostdlib -O2 -D__FPU_PRESENT -gdwarf-3 -fstack-usage -fdata-sections -nostartfiles -nostdlib -Wall -Wpointer-arith -Wstrict-prototypes -Wundef -Wno-write-strings -Wno-maybe-uninitialized -c -MMD -Wextra
CFLAGS += -Wl,--start-group $(INC) -Wl,--end-group 
CFLAGS += -DARDUINO_SDK -DCONFIG_PLATFORM_8721D -DCONFIG_USE_MBEDTLS_ROM_ALG -DCONFIG_FUNCION_O0_OPTIMIZE -DDM_ODM_SUPPORT_TYPE=32 -I.
CFLAGS += main.c -o build/main.o

# Optimize level
#CFLAGS = -O2

# CPU arch
#CFLAGS += -march=armv8-m.main+dsp
#CFLAGS += -mthumb 
#CFLAGS += -D$(CHIP)
#CFLAGS += -DCONFIG_PLATFORM_8721D
# source code macro
#CFLAGS += -ffunction-sections -mcmse -mfloat-abi=hard -mfpu=fpv5-sp-d16 -g -gdwarf-3 
#CFLAGS += -nostartfiles -nodefaultlibs -nostdlib -O2 -D__FPU_PRESENT -gdwarf-3 -fstack-usage 
#CFLAGS += -fdata-sections -nostartfiles -nostdlib -Wall -Wpointer-arith -Wstrict-prototypes 
#CFLAGS += -Wundef -Wno-write-strings -Wno-maybe-uninitialized -c -MMD -Wextra 
#CFLAGS += -Wl,--start-group
#CFLAGS += $(INC)
#CFLAGS += -Wl,--end-group

###########################
#         LDFLAGS         #
###########################
LFLAGS = -O2 -march=armv8-m.main+dsp -mthumb -mcmse -mfloat-abi=hard -mfpu=fpv5-sp-d16 -nostartfiles -specs nosys.specs -Wl,--gc-sections
LIBFLAGS = -Wl,--cref -Wl,--build-id=none -Wl,-wrap,strcat -Wl,-wrap,strchr -Wl,-wrap,strcmp -Wl,-wrap,strncmp -Wl,-wrap,strcpy -Wl,-wrap,strncpy -Wl,-wrap,strlen -Wl,-wrap,strnlen -Wl,-wrap,strncat -Wl,-wrap,strpbrk -Wl,-wrap,strstr -Wl,-wrap,strtok -Wl,-wrap,strsep -Wl,-wrap,strtoll -Wl,-wrap,strtoul -Wl,-wrap,strtoull -Wl,-wrap,atoi -Wl,-wrap,malloc -Wl,-wrap,free -Wl,-wrap,realloc -Wl,-wrap,memcmp -Wl,-wrap,memcpy -Wl,-wrap,memmove -Wl,-wrap,memset -Wl,-wrap,printf -Wl,-wrap,sprintf -Wl,-wrap,snprintf -Wl,--no-enum-size-warning -Wl,--warn-common 

#LFLAGS =
#LFLAGS += -Wl,--cref -Wl,--build-id=none -Wl,-wrap,strcat -Wl,-wrap,strchr -Wl,-wrap,strcmp -Wl,-wrap,strncmp -Wl,-wrap,strcpy -Wl,-wrap,strncpy -Wl,-wrap,strlen -Wl,-wrap,strnlen -Wl,-wrap,strncat -Wl,-wrap,strpbrk -Wl,-wrap,strstr -Wl,-wrap,strtok -Wl,-wrap,strsep -Wl,-wrap,strtoll -Wl,-wrap,strtoul -Wl,-wrap,strtoull -Wl,-wrap,atoi -Wl,-wrap,malloc -Wl,-wrap,free -Wl,-wrap,realloc -Wl,-wrap,memcmp -Wl,-wrap,memcpy -Wl,-wrap,memmove -Wl,-wrap,memset -Wl,-wrap,printf -Wl,-wrap,sprintf -Wl,-wrap,snprintf -Wl,--no-enum-size-warning -Wl,--warn-common
#LFLAGS += -O2 -march=armv8-m.main+dsp -mthumb -mcmse -mfloat-abi=hard -mfpu=fpv5-sp-d16 
#LFLAGS += -nostartfiles -specs nosys.specs -Wl,--gc-sections

#LIBFLAGS = -Wl,--cref -Wl,--build-id=none 
#LIBFLAGS = -Wl,--no-enum-size-warning -Wl,--warn-common


###############################
#         ARCHIVE LIST        #
###############################
LIBAR = -Wl,--start-group
LIBAR += $(LD_PATH)/ARCHIVE/btgap.a 
LIBAR += $(LD_PATH)/ARCHIVE/btmesh_dev.a
LIBAR += $(LD_PATH)/ARCHIVE/btmesh_prov.a
LIBAR += $(LD_PATH)/ARCHIVE/lib_cmsis_dsp.a
LIBAR += $(LD_PATH)/ARCHIVE/lib_coap.a
LIBAR += $(LD_PATH)/ARCHIVE/lib_dct.a
LIBAR += $(LD_PATH)/ARCHIVE/lib_eap.a
LIBAR += $(LD_PATH)/ARCHIVE/lib_http2.a
LIBAR += $(LD_PATH)/ARCHIVE/lib_httpc.a
LIBAR += $(LD_PATH)/ARCHIVE/lib_httpd.a
LIBAR += $(LD_PATH)/ARCHIVE/lib_m4a_self.a
LIBAR += $(LD_PATH)/ARCHIVE/lib_mdns.a
LIBAR += $(LD_PATH)/ARCHIVE/lib_tftp.a
LIBAR += $(LD_PATH)/ARCHIVE/lib_usbd.a
LIBAR += $(LD_PATH)/ARCHIVE/lib_usbh.a
LIBAR += $(LD_PATH)/ARCHIVE/lib_user.a
LIBAR += $(LD_PATH)/ARCHIVE/lib_websocket.a
LIBAR += $(LD_PATH)/ARCHIVE/lib_wifi_fw.a
LIBAR += $(LD_PATH)/ARCHIVE/lib_wifi_ucps_fw.a
LIBAR += $(LD_PATH)/ARCHIVE/lib_wlan.a
LIBAR += $(LD_PATH)/ARCHIVE/lib_wps.a
LIBAR += $(LD_PATH)/ARCHIVE/lib_bt.a
LIBAR += $(LD_PATH)/ARCHIVE/btgap.a
LIBAR += $(LD_PATH)/ARCHIVE/lib_arduino.a
LIBAR += $(LD_PATH)/ARCHIVE/lib_mbedtls240.a
LIBAR += -Wl,--end-group


###########################
#         BUILD RULES     #
###########################
application: prerequirement $(UPY_O)
	$(Q)echo '==========================================================='
	$(Q)echo 'Linking $(CHIP)'
	$(Q)echo '==========================================================='
	$(LD) -L$(TC_PATH)../lib -L$(LD_PATH) -T$(LD_PATH)/rlx8721d_img2_is_arduino.ld $(LFLAGS) -Wl,-Map=$(BUILD)/Preprocessed_image2.map $(LIBFLAGS) -o $(BUILD)/$(TARGET).axf -Wl,--start-group $(OBJ) -Wl,--end-group $(LIBAR) -lm -lstdc++
	$(Q)$(OBJDUMP) -d $(BUILD)/$(TARGET).axf > $(BUILD)/Preprocessed_image2.asm


.PHONY: manipulate_images
manipulate_images: $(POSTBUILD)
	$(Q)echo '==========================================================='
	$(Q)echo 'Image manipulating'
	$(Q)echo '==========================================================='
	$(Q)$(BUILD)/$(POSTBUILD) $(BUILD) $(TARGET).axf ../$(TC_PATH) 0
	$(Q)echo '===========================''
	$(Q)echo  End of Image manipulating
	$(Q)echo '===========================''


.PHONY: prerequirement
prerequirement:
	$(Q)echo '==========================================================='
	$(Q)echo 'Prepare tools and images'
	$(Q)echo '==========================================================='
	$(Q)mkdir -p $(BUILD)/bsp/image
	$(Q)cp -f $(TOOL)/image/km0_boot_all.bin $(BUILD)/bsp/image/km0_boot_all.bin
	$(Q)chmod +rw $(BUILD)/bsp/image/km0_boot_all.bin
	$(Q)cp -f $(TOOL)/image/km0_image2_all.bin $(BUILD)/bsp/image/km0_image2_all.bin
	$(Q)chmod +rw $(BUILD)/bsp/image/km0_image2_all.bin
	$(Q)cp -f $(TOOL)/image/km4_boot_all.bin $(BUILD)/bsp/image/km4_boot_all.bin
	$(Q)chmod +rw $(BUILD)/bsp/image/km4_boot_all.bin
	$(Q)mkdir -p $(BUILD)/$(BUILDTOOL_PATH)
	$(Q)cp -f $(TOOL)/$(PICK) $(BUILD)/$(BUILDTOOL_PATH)/$(PICK)
	$(Q)cp -f $(TOOL)/$(PAD) $(BUILD)/$(BUILDTOOL_PATH)/$(PAD)


######################
#    Compilation     #
######################
$(UPY_O): $(BUILD)/%.o : %.c
	$(CC) $(CFLAGS)
#	$(CC) $(CFLAGS) -c $< -o $@


##############
#    Tools   #
##############
$(POSTBUILD):
	$(Q)cp -f $(TOOL)/$(POSTBUILD) $(BUILD)/$(POSTBUILD)

$(IMAGETOOL):
	$(Q)cp -f $(TOOL)/$(IMAGETOOL) $(BUILD)/$(IMAGETOOL)
	$(Q)cp -f $(TOOL)/imgtool_flashloader_amebad.bin ./
	$(Q)cp -f $(TOOL)/image/km0_boot_all.bin ./
	$(Q)cp -f $(TOOL)/image/km0_image2_all.bin ./
	$(Q)cp -f $(TOOL)/image/km4_boot_all.bin ./
	$(Q)cp -f $(BUILD)/km0_km4_image2.bin ./

.PHONY: cleanpwd
cleanpwd:
	rm -f ./*.d
	rm -f ./*.bin


.PHONY: upload
upload: $(IMAGETOOL)
	$(BUILD)/$(IMAGETOOL) /dev/ttyUSB0

.PHONY: com
com:
	picocom -b115200 /dev/ttyUSB0

.PHONY: clean
clean:
	rm -rf build
	rm -f *.bin
	clear


#include $(TOP)/py/mkrules.mk
