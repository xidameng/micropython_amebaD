

################################
#        Include path          # 
################################
INC += -I.
INC += -I$(VENDOR)/project/realtek_amebaD_va0_example/inc/inc_hp
INC += -Iinc/ameba
INC += -I$(TOP)
INC += -I$(BUILD)
INC += -I$(HEADER_BUILD)

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
#INC += -I$(TOP)/lib/lwip/src/include/lwip

INC += -Imphelper
INC += -Imphelper/amebad
INC += -Imphelper/gccollect
INC += -Imphelper/mods
INC += -Imphelper/mods/network
INC += -Imphelper/mods/machine


ifeq ($(CHIP), AMEBAD)
# Source file list
# -------------------------------------------------------------------
# micropython source
UPY_C += pins.c
#UPY_C += mphelper/diskio.c
UPY_C += mphelper/exception.c
UPY_C += mphelper/help.c
UPY_C += mphelper/amebad/mphal.c
UPY_C += mphelper/amebad/objloguart.c
UPY_C += mphelper/input.c
UPY_C += mphelper/bufhelper.c
UPY_C += mphelper/mpthreadport.c
UPY_C += mphelper/gccollect/gccollect.c
UPY_C += mphelper/mods/modameba.c
UPY_C += mphelper/mods/modmachine.c
#UPY_C += mphelper/mods/modlwip.c
#UPY_C += mphelper/mods/moduwireless.c
#UPY_C += mphelper/mods/modnetwork.c
UPY_C += mphelper/mods/modutime.c
UPY_C += mphelper/mods/modterm.c
UPY_C += mphelper/mods/moduos.c
#UPY_C += mphelper/mods/modussl.c
#UPY_C += mphelper/mods/machine/objwdt.c
#UPY_C += mphelper/mods/machine/objflash.c
UPY_C += mphelper/mods/machine/objrtc.c
#UPY_C += mphelper/mods/machine/objadc.c
##UPY_C += mphelper/mods/machine/objdac.c
UPY_C += mphelper/mods/machine/objpin.c
#UPY_C += mphelper/mods/machine/obji2c.c
#UPY_C += mphelper/mods/machine/objpwm.c
UPY_C += mphelper/mods/machine/objtimer.c
##UPY_C += mphelper/mods/machine/objspi.c
UPY_C += mphelper/mods/machine/objuart.c
#UPY_C += mphelper/mods/machine/objcrypto.c
#UPY_C += mphelper/mods/wireless/objwlan.c
#UPY_C += mphelper/mods/network/objnetif.c
#UPY_C += mphelper/mods/network/dhcps.c
UPY_C += lib/utils/pyexec.c
UPY_C += lib/mp-readline/readline.c
UPY_C += lib/utils/interrupt_char.c
UPY_C += lib/timeutils/timeutils.c
UPY_C += lib//utils/sys_stdio_mphal.c
#UPY_C += lib/netutils/netutils.c 
#UPY_C += lib/oofatfs/ff.c 
#UPY_C += lib/oofatfs/ffunicode.c 
#UPY_C += lib/oofatfs/option/ccsbcs.c 
endif
UPY_C += mphelper/amebad/ringbuffer.c
UPY_C += main.c


# Initialize target name and target object files
# -------------------------------------------------------------------

all: application manipulate_images

TARGET=application


# Generate obj list
# -------------------------------------------------------------------

UPY_O = $(addprefix $(BUILD)/, $(UPY_C:.c=.o))

OBJ = $(UPY_O) $(PY_O)
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
CFLAGS += -D$(CHIP)
CFLAGS += -DCONFIG_PLATFORM_8721D
# source code macro
CFLAGS += -ffunction-sections -mcmse -mfloat-abi=hard -mfpu=fpv5-sp-d16 -g -gdwarf-3 
CFLAGS += -nostartfiles -nodefaultlibs -nostdlib -O2 -D__FPU_PRESENT -gdwarf-3 -fstack-usage 
CFLAGS += -fdata-sections -nostartfiles -nostdlib -Wall -Wpointer-arith -Wstrict-prototypes 
CFLAGS += -Wundef -Wno-write-strings -Wno-maybe-uninitialized -c -MMD -Wextra 
CFLAGS += -Wl,--start-group
CFLAGS += $(INC)
CFLAGS += -Wl,--end-group

###########################
#         LDFLAGS         #
###########################
LFLAGS =
LFLAGS += -O2 -march=armv8-m.main+dsp -mthumb -mcmse -mfloat-abi=hard -mfpu=fpv5-sp-d16 
LFLAGS += -nostartfiles -specs nosys.specs -Wl,--gc-sections

LIBFLAGS = -Wl,--no-enum-size-warning -Wl,--warn-common


###############################
#         ARCHIVE LIST        #
###############################
LIBAR += -L$(VENDOR)/ARCHIVE_LIB/ -l_arduino -l_wlan -l_wps -l_wlan_mp -l_wifi_ucps_fw 
LIBAR += -l_wifi_fw -l_websocket -l_user -l_usbh -l_usbd -l_tftp -l_mdns -l_m4a_self 
LIBAR += -l_httpd -l_httpc -l_http2 -l_eap -l_dct -l_coap -l_cmsis_dsp -l_bt 


###########################
#         BUILD RULES     #
###########################
application: prerequirement $(PY_O) $(UPY_O)
	$(Q)echo '==========================================================='
	$(Q)echo 'Linking $(CHIP)'
	$(Q)echo '==========================================================='
	$(LD) -L$(TOOL) -L$(TC_PATH)../lib -T$(TOOL)/rlx8721d_img2_is_arduino.ld $(LFLAGS) -Wl,-Map=$(BUILD)/Preprocessed_image2.map $(LIBFLAGS) -o $(BUILD)/$(TARGET).axf $(OBJ) $(LIBAR) -lm
	$(Q)$(OBJDUMP) -d $(BUILD)/$(TARGET).axf > $(BUILD)/Preprocessed_image2.asm


.PHONY: manipulate_images
manipulate_images: $(POSTBUILD)
	$(Q)echo '==========================================================='
	$(Q)echo 'Image manipulating'
	$(Q)echo '==========================================================='
	$(Q)./$(BUILD)/$(POSTBUILD) $(BUILD) $(TARGET).axf ../$(TC_PATH) 0
	$(Q)echo '==========================='
	$(Q)echo  End of Image manipulating
	$(Q)echo '==========================='


.PHONY: prerequirement
prerequirement: check_toolchain check_postbuildtools
	$(Q)echo '==========================================================='
	$(Q)echo 'Prepare tools and images'
	$(Q)echo '==========================================================='
	$(Q)$(MKDIR) -p $(BUILD)/bsp/image
	$(Q)cp -f $(TOOL)/image/km0_boot_all.bin $(BUILD)/bsp/image/km0_boot_all.bin
	$(Q)chmod +rw $(BUILD)/bsp/image/km0_boot_all.bin
	$(Q)cp -f $(TOOL)/image/km0_image2_all.bin $(BUILD)/bsp/image/km0_image2_all.bin
	$(Q)chmod +rw $(BUILD)/bsp/image/km0_image2_all.bin
	$(Q)cp -f $(TOOL)/image/km4_boot_all.bin $(BUILD)/bsp/image/km4_boot_all.bin
	$(Q)chmod +rw $(BUILD)/bsp/image/km4_boot_all.bin
	$(Q)mkdir -p $(BUILD)/$(BUILDTOOL_PATH)
	$(Q)cp -f $(POSTBUILDTOOL_PATH)/$(PICK) $(BUILD)/$(BUILDTOOL_PATH)/$(PICK)
	$(Q)cp -f $(POSTBUILDTOOL_PATH)/$(PAD) $(BUILD)/$(BUILDTOOL_PATH)/$(PAD)

.PHONY: check_toolchain
check_toolchain:
	@if [ -d $(TC_PATH) ]; \
		then echo "--ToolChain Exists--"; \
		else echo "Extracting toolchain..."; \
			make -C tool/toolchain all; fi

.PHONY: check_postbuildtools
check_postbuildtools:
	@if [ -d $(POSTBUILDTOOL_PATH) ]; \
		then echo "--Postbuildtool Exists--"; \
		else echo "Extracting tools..."; \
			make -C $(TOOL) all; fi

######################
#    Compilation     #
######################
$(UPY_O): $(BUILD)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@


##############
#    Tools   #
##############
$(POSTBUILD):
	$(Q)cp -f $(POSTBUILDTOOL_PATH)/$(POSTBUILD) $(BUILD)/$(POSTBUILD)

$(IMAGETOOL):
	$(Q)cp -f $(POSTBUILDTOOL_PATH)/$(IMAGETOOL) $(BUILD)/$(IMAGETOOL)
	$(Q)cp -f $(POSTBUILDTOOL_PATH)/imgtool_flashloader_amebad.bin ./
	$(Q)cp -f $(TOOL)/image/km0_boot_all.bin ./
	$(Q)cp -f $(TOOL)/image/km0_image2_all.bin ./
	$(Q)cp -f $(TOOL)/image/km4_boot_all.bin ./
	$(Q)cp -f $(BUILD)/km0_km4_image2.bin ./


.PHONY: upload
upload: $(IMAGETOOL)
	./$(BUILD)/$(IMAGETOOL) $(UPLOAD_PATH)


.PHONY: com
com:
	picocom -b115200 $(UPLOAD_PATH)


.PHONY: purge
purge:
	make cleanpwd
	make clean
	clear


.PHONY: cleanpwd
cleanpwd:
	rm -f ./*.d
	rm -f ./*.bin
