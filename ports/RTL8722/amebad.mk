AMEBASIZE = $(BUILD)/amebasize
POSTBUILD = $(BUILD)/postbuild_img2_arduino_linux

################################
#        Include path          # 
################################
INC = -I.
INC += -I$(VENDOR)/project/realtek_amebaD_va0_example/inc/inc_hp
INC += -Iinc/include/amebad
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

INC += -Imp/mphelper
INC += -Imp/mphelper/amebad
INC += -Imp/mphelper/gccollect
INC += -Imp/mphelper/mods
INC += -Imp/mphelper/mods/network
INC += -Imp/mphelper/mods/machine
INC += -Imp/mpconfigports


# Source file list
# -------------------------------------------------------------------
# micropython source
UPY_C += pins.c
UPY_C += mphelper/diskio.c
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
#UPY_C += mphelper/mods/modutime.c
UPY_C += mphelper/mods/modterm.c
UPY_C += mphelper/mods/moduos.c
#UPY_C += mphelper/mods/modussl.c
##UPY_C += mphelper/mods/machine/objloguart.c
#UPY_C += mphelper/mods/machine/objwdt.c
#UPY_C += mphelper/mods/machine/objflash.c
#UPY_C += mphelper/mods/machine/objrtc.c
#UPY_C += mphelper/mods/machine/objadc.c
##UPY_C += mphelper/mods/machine/objdac.c
UPY_C += mphelper/mods/machine/objpin.c
#UPY_C += mphelper/mods/machine/obji2c.c
#UPY_C += mphelper/mods/machine/objpwm.c
#UPY_C += mphelper/mods/machine/objtimer.c
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

UPY_C += main.c


# Initialize target name and target object files
# -------------------------------------------------------------------

all: application manipulate_images

TARGET=application

# Include folder list
# -------------------------------------------------------------------

# Generate obj list
# -------------------------------------------------------------------

UPY_O = $(addprefix $(BUILD)/, $(UPY_C:.c=.o))

OBJ = $(UPY_O) $(PY_O) 
SRC_QSTR += $(UPY_C)
SRC_QSTR_AUTO_DEPS +=

################################
# 			CFLAGS 			   #
################################


CFLAGS += -ffunction-sections 
CFLAGS += -march=armv8-m.main+dsp
CFLAGS += -mthumb -mcmse -mfloat-abi=hard -mfpu=fpv5-sp-d16 -g -gdwarf-3 
CFLAGS += -nostartfiles -nodefaultlibs -nostdlib -O2 -D__FPU_PRESENT -gdwarf-3 -fstack-usage 
CFLAGS += -fdata-sections -nostartfiles -nostdlib -Wall -Wpointer-arith -Wstrict-prototypes 
CFLAGS += -Wundef -Wno-write-strings -Wno-maybe-uninitialized -c -MMD -Wextra 
CFLAGS += -DAMEBAD
CFLAGS += -DARDUINO_SDK -DCONFIG_PLATFORM_8721D
CFLAGS += -DCONFIG_USE_MBEDTLS_ROM_ALG -DCONFIG_FUNCION_O0_OPTIMIZE -DDM_ODM_SUPPORT_TYPE=32 
CFLAGS += $(INC)



###########################
#         LDFLAGS         #
###########################
LFLAGS =
LFLAGS += -march=armv8-m.main+dsp -mthumb -mcmse -mfloat-abi=hard -mfpu=fpv5-sp-d16 
LFLAGS += -nostartfiles -specs nosys.specs -Wl,--warn-section-align -Wl,--gc-sections -O2  

LIBFLAGS += -Wl,--cref -Wl,--build-id=none 
LIBFLAGS += -Wl,--no-enum-size-warning -Wl,--warn-common


###############################
#         ARCHIVE LIST        #
###############################
LIBAR += -L$(LD_PATH)/ARCHIVE -l_arduino -l_wlan -l_wps -l_wlan_mp -l_wifi_ucps_fw 
LIBAR += -l_wifi_fw -l_websocket -l_user -l_usbh -l_usbd -l_tftp -l_mdns -l_m4a_self 
LIBAR += -l_httpd -l_httpc -l_http2 -l_eap -l_dct -l_coap -l_cmsis_dsp -l_bt


###########################
#         BUILD RULES     #
###########################
application: prerequirement $(UPY_O) $(PY_O) 
	$(ECHO) "Building $(CHIP) "
	$(Q)$(LD) -L$(LD_PATH) -T$(LD_PATH)/rlx8721d_img2_is_arduino.ld $(LFLAGS) -Wl,-Map=$(BUILD)/$(TARGET).map $(LIBFLAGS) -o $(BUILD)/$(TARGET).axf $(OBJ) $(LIBAR) -lm 
	$(Q)$(OBJDUMP) -d $(BUILD)/$(TARGET).axf > $(BUILD)/$(TARGET).asm

#$(Q)$(LD) -L$(LD_PATH) -T$(LD_PATH)/rlx8721d_img2_is_arduino.ld $(LFLAGS) -Wl,-Map=$(BUILD)/mp.map $(LIBFLAGS) -o $(BUILD)/$(TARGET).axf -Wl,--start-group $(BUILD)/*.o -Wl,--end-group -Wl,--start-group -Wl,--whole-archive -Wl,--no-whole-archive $(LIBAR) -Wl,--end-group -lm 
#-Wl,--start-group $(BUILD)/py/*.o -Wl,--end-group -Wl,--start-group $(BUILD)/lib/embed/*.o -Wl,--end-group -Wl,--start-group $(BUILD)/lib/mp-readline/*.o -Wl,--end-group -Wl,--start-group $(BUILD)/lib/netutils/*.o -Wl,--end-group -Wl,--start-group $(BUILD)/lib/oofatfs/*.o -Wl,--end-group -Wl,--start-group $(BUILD)/lib/timeutils/*.o -Wl,--end-group -Wl,--start-group $(BUILD)/lib/utils/*.o -Wl,--end-group \

.PHONY: manipulate_images
manipulate_images: $(POSTBUILD)
	$(Q)echo ===========================================================
	$(Q)echo Image manipulating
	$(Q)echo ===========================================================
	
	$(Q)$(POSTBUILD) $(BUILD) $(TARGET).axf ../$(TC_PATH) 0

	$(Q)echo ===========================
	$(Q)echo  End of Image manipulating
	$(Q)echo ===========================


.PHONY: prerequirement
prerequirement:
	$(Q)echo ===========================================================
	$(Q)echo Build $(TARGET)
	$(Q)echo ===========================================================
	$(Q)$(MKDIR) -p $(BUILD)/bsp/image
	$(Q)cp -f $(TOOL)/image/km0_boot_all.bin $(BUILD)/bsp/image/km0_boot_all.bin
	$(Q)chmod +rw $(BUILD)/bsp/image/km0_boot_all.bin
	$(Q)cp -f $(TOOL)/image/km0_image2_all.bin $(BUILD)/bsp/image/km0_image2_all.bin
	$(Q)chmod +rw $(BUILD)/bsp/image/km0_image2_all.bin
	$(Q)cp -f $(TOOL)/image/km4_boot_all.bin $(BUILD)/bsp/image/km4_boot_all.bin
	$(Q)chmod +rw $(BUILD)/bsp/image/km4_boot_all.bin
	$(Q)$(MKDIR) -p $(BUILD)/tools/linux
	$(Q)cp -f $(TOOL)/pick $(BUILD)/tools/linux/pick
	$(Q)cp -f $(TOOL)/pad $(BUILD)/tools/linux/pad


##############
#    Tools   #
##############
$(PICK):
	$(Q)cp -f $(TOOL)/pick $(BUILD)/tools/linux/pick

$(PAD):
	$(Q)cp -f $(TOOL)/pad $(BUILD)/tools/linux/pad

$(AMEBASIZE):
	$(Q)cp -f $(TOOL)/amebasize $(BUILD)/amebasize

$(POSTBUILD):
	$(Q)cp -f $(TOOL)/postbuild_img2_arduino_linux $(BUILD)/postbuild_img2_arduino_linux

$(UPY_O): $(BUILD)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
	

.PHONY: cleanD
cleanD:
	rm ./*.d