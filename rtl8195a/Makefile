include ../py/mkenv.mk

# qstr definitions (must come before including py.mk)
#QSTR_DEFS += qstrdefsport.h $(HEADER_BUILD)/pins_qstr.h
QSTR_DEFS += qstrdefsport.h

-include mpconfigport.mk

# include py core make definitions
include ../py/py.mk

OS := $(shell uname)
VENDOR := vendor
GDB_SERVER := openocd
TOOL := tool

.PHONY: toolchain
toolchain:
ifeq ($(OS),Linux)
	if [ ! -d $(VENDOR)/tools/arm-none-eabi-gcc/4.8.3-2014q1 ] ; then tar -jxf $(VENDOR)/tools/arm-none-eabi-gcc/gcc-arm-none-eabi-4_8-2014q1-20140314-linux.tar.bz2 -C $(VENDOR)/tools/arm-none-eabi-gcc/ ; mv $(VENDOR)/tools/arm-none-eabi-gcc/gcc-arm-none-eabi-4_8-2014q1 $(VENDOR)/tools/arm-none-eabi-gcc/4.8.3-2014q1 ; fi
else
	if [ ! -d $(VENDOR)/tools/arm-none-eabi-gcc/4.8.3-2014q1 ] ; then tar -zxf $(VENDOR)/tools/arm-none-eabi-gcc/4.8.3-2014q1.tar.gz -C $(VENDOR)/tools/arm-none-eabi-gcc/ ; fi
endif

# Initialize tool chain
# -------------------------------------------------------------------
ARM_GCC_TOOLCHAIN = $(VENDOR)/tools/arm-none-eabi-gcc/4.8.3-2014q1/
AMEBA_TOOLDIR = $(VENDOR)/component/soc/realtek/8195a/misc/iar_utility/common/tools/
FLASH_TOOLDIR = $(VENDOR)/component/soc/realtek/8195a/misc/gcc_utility/

CROSS_COMPILE = $(ARM_GCC_TOOLCHAIN)bin/arm-none-eabi-

# Compilation tools
AR = $(CROSS_COMPILE)ar
CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
NM = $(CROSS_COMPILE)nm
LD = $(CROSS_COMPILE)gcc
GDB = $(CROSS_COMPILE)gdb
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

#OBJ_DIR=$(TARGET)/Debug/obj
#BIN_DIR=$(TARGET)/Debug/bin
OBJ_DIR=$(BUILD)
BIN_DIR=$(BUILD)

# Include folder list
# -------------------------------------------------------------------

INCLUDES =
INCLUDES += -I.
INCLUDES += -I..
INCLUDES += -Iinc
INCLUDES += -I$(BUILD)
INCLUDES += -I$(HEADER_BUILD)
INCLUDES += -I$(VENDOR)/component/soc/realtek/common/bsp
INCLUDES += -I$(VENDOR)/component/os/freertos
INCLUDES += -I$(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/include
INCLUDES += -I$(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/portable/GCC/ARM_CM3
INCLUDES += -I$(VENDOR)/component/os/os_dep/include
INCLUDES += -I$(VENDOR)/component/soc/realtek/8195a/misc/driver
INCLUDES += -I$(VENDOR)/component/common/api/network/include
INCLUDES += -I$(VENDOR)/component/common/api
INCLUDES += -I$(VENDOR)/component/common/api/platform
INCLUDES += -I$(VENDOR)/component/common/api/wifi
INCLUDES += -I$(VENDOR)/component/common/api/wifi/rtw_wpa_supplicant/src
#INCLUDES += -I$(VENDOR)/component/common/application
#INCLUDES += -I$(VENDOR)/component/common/application/iotdemokit
#INCLUDES += -I$(VENDOR)/component/common/application/google
INCLUDES += -I$(VENDOR)/component/common/media/framework
INCLUDES += -I$(VENDOR)/component/common/example
INCLUDES += -I$(VENDOR)/component/common/example/wlan_fast_connect
INCLUDES += -I$(VENDOR)/component/common/mbed/api
INCLUDES += -I$(VENDOR)/component/common/mbed/hal
INCLUDES += -I$(VENDOR)/component/common/mbed/hal_ext
INCLUDES += -I$(VENDOR)/component/common/mbed/targets/hal/rtl8195a
INCLUDES += -I$(VENDOR)/component/common/network
INCLUDES += -I$(VENDOR)/component/common/network/lwip/lwip_v1.4.1/port/realtek/freertos
INCLUDES += -I$(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/include
INCLUDES += -I$(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/include/lwip
INCLUDES += -I$(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/include/ipv4
INCLUDES += -I$(VENDOR)/component/common/network/lwip/lwip_v1.4.1/port/realtek
INCLUDES += -I$(VENDOR)/component/common/test
INCLUDES += -I$(VENDOR)/component/soc/realtek/8195a/cmsis
INCLUDES += -I$(VENDOR)/component/soc/realtek/8195a/cmsis/device
INCLUDES += -I$(VENDOR)/component/soc/realtek/8195a/fwlib
INCLUDES += -I$(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a
INCLUDES += -I$(VENDOR)/component/soc/realtek/8195a/misc/rtl_std_lib/include
INCLUDES += -I$(VENDOR)/component/common/drivers/wlan/realtek/include
INCLUDES += -I$(VENDOR)/component/common/drivers/wlan/realtek/src/osdep
INCLUDES += -I$(VENDOR)/component/common/drivers/wlan/realtek/src/hci
INCLUDES += -I$(VENDOR)/component/common/drivers/wlan/realtek/src/hal
INCLUDES += -I$(VENDOR)/component/common/drivers/wlan/realtek/src/hal/OUTSRC
INCLUDES += -I$(VENDOR)/component/soc/realtek/8195a/fwlib/ram_lib/wlan/realtek/wlan_ram_map/rom
INCLUDES += -I$(VENDOR)/component/common/network/ssl/polarssl-1.3.8/include
INCLUDES += -I$(VENDOR)/component/common/network/ssl/ssl_ram_map/rom
INCLUDES += -I$(VENDOR)/component/common/utilities
INCLUDES += -I$(VENDOR)/component/soc/realtek/8195a/misc/rtl_std_lib/include
#INCLUDES += -I$(VENDOR)/component/common/application/apple/WACServer/External/Curve25519
#INCLUDES += -I$(VENDOR)/component/common/application/apple/WACServer/External/GladmanAES
INCLUDES += -I$(VENDOR)/component/soc/realtek/8195a/fwlib/ram_lib/usb_otg/include
INCLUDES += -I$(VENDOR)/component/common/video/v4l2/inc
INCLUDES += -I$(VENDOR)/component/common/media/codec
INCLUDES += -I$(VENDOR)/component/common/drivers/usb_class/host/uvc/inc
INCLUDES += -I$(VENDOR)/component/common/drivers/usb_class/device
INCLUDES += -I$(VENDOR)/component/common/drivers/usb_class/device/class
#INCLUDES += -I$(VENDOR)/component/common/file_system/fatfs
#INCLUDES += -I$(VENDOR)/component/common/file_system/fatfs/r0.10c/include
INCLUDES += -I$(VENDOR)/component/common/drivers/sdio/realtek/sdio_host/inc
INCLUDES += -I$(VENDOR)/component/common/audio
INCLUDES += -I$(VENDOR)/component/common/drivers/i2s
#INCLUDES += -I$(VENDOR)/component/common/application/xmodem

INCLUDES += -I../extmod
INCLUDES += -I../lib/utils
INCLUDES += -I../lib/timeutils
INCLUDES += -I../lib/mp-readline
INCLUDES += -I../lib/netutils

INCLUDES += -I./mphelper
INCLUDES += -I./mphelper/gccollect
INCLUDES += -I./mphelper/hal
INCLUDES += -I./mphelper/mods
INCLUDES += -I./mphelper/mods/network
INCLUDES += -I./mphelper/mods/machine


# Source file list
# -------------------------------------------------------------------

SRC_C =
DRAM_C =
#cmsis
SRC_C += $(VENDOR)/component/soc/realtek/8195a/cmsis/device/system_8195a.c

#console
#SRC_C += $(VENDOR)/component/common/api/at_cmd/atcmd_ethernet.c
#SRC_C += $(VENDOR)/component/common/api/at_cmd/atcmd_lwip.c
#SRC_C += $(VENDOR)/component/common/api/at_cmd/atcmd_sys.c
#SRC_C += $(VENDOR)/component/common/api/at_cmd/atcmd_wifi.c
#SRC_C += $(VENDOR)/component/common/api/at_cmd/log_service.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/misc/driver/low_level_io.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/misc/driver/rtl_consol.c

#network - api
SRC_C += $(VENDOR)/component/common/api/wifi/rtw_wpa_supplicant/wpa_supplicant/wifi_eap_config.c
SRC_C += $(VENDOR)/component/common/api/wifi/rtw_wpa_supplicant/wpa_supplicant/wifi_p2p_config.c
SRC_C += $(VENDOR)/component/common/api/wifi/rtw_wpa_supplicant/wpa_supplicant/wifi_wps_config.c
SRC_C += $(VENDOR)/component/common/api/wifi/wifi_conf.c
SRC_C += $(VENDOR)/component/common/api/wifi/wifi_ind.c
SRC_C += $(VENDOR)/component/common/api/wifi/wifi_promisc.c
SRC_C += $(VENDOR)/component/common/api/wifi/wifi_simple_config.c
SRC_C += $(VENDOR)/component/common/api/wifi/wifi_util.c
SRC_C += $(VENDOR)/component/common/api/lwip_netconf.c

#network - app
SRC_C += $(VENDOR)/component/common/api/network/src/ping_test.c
SRC_C += $(VENDOR)/component/common/utilities/ssl_client.c
SRC_C += $(VENDOR)/component/common/utilities/ssl_client_ext.c
SRC_C += $(VENDOR)/component/common/utilities/tcptest.c
#SRC_C += $(VENDOR)/component/common/application/uart_adapter/uart_adapter.c
SRC_C += $(VENDOR)/component/common/utilities/uart_ymodem.c
SRC_C += $(VENDOR)/component/common/utilities/update.c
SRC_C += $(VENDOR)/component/common/api/network/src/wlan_network.c

#network - lwip
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/api/api_lib.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/api/api_msg.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/api/err.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/api/netbuf.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/api/netdb.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/api/netifapi.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/api/sockets.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/api/tcpip.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/ipv4/autoip.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/ipv4/icmp.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/ipv4/igmp.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/ipv4/inet.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/ipv4/inet_chksum.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/ipv4/ip.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/ipv4/ip_addr.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/ipv4/ip_frag.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/def.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/dhcp.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/dns.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/init.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/lwip_timers.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/mem.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/memp.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/netif.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/pbuf.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/raw.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/stats.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/sys.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/tcp.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/tcp_in.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/tcp_out.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/udp.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/netif/etharp.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/port/realtek/freertos/ethernetif.c
SRC_C += $(VENDOR)/component/common/drivers/wlan/realtek/src/osdep/lwip_intf.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/port/realtek/freertos/sys_arch.c
SRC_C += $(VENDOR)/component/common/network/dhcp/dhcps.c
SRC_C += $(VENDOR)/component/common/network/sntp/sntp.c

#network - mdns
SRC_C += $(VENDOR)/component/common/network/mDNS/mDNSPlatform.c


#os - freertos
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/portable/MemMang/heap_5.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/portable/GCC/ARM_CM3/port.c
SRC_C += $(VENDOR)/component/os/freertos/cmsis_os.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/croutine.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/event_groups.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/list.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/queue.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/tasks.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/timers.c

#os - osdep
SRC_C += $(VENDOR)/component/os/os_dep/device_lock.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_service.c
SRC_C += $(VENDOR)/component/os/os_dep/mailbox.c
SRC_C += $(VENDOR)/component/os/os_dep/osdep_api.c
SRC_C += $(VENDOR)/component/os/os_dep/osdep_service.c
SRC_C += $(VENDOR)/component/os/os_dep/tcm_heap.c

#peripheral - api
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/analogin_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/dma_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/efuse_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/ethernet_api.c
SRC_C += $(VENDOR)/component/common/drivers/ethernet_mii/ethernet_mii.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/flash_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/gpio_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/gpio_irq_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/i2c_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/i2s_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/log_uart_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/nfc_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/pinmap.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/pinmap_common.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/port_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/pwmout_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/rtc_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/serial_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/sleep.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/spdio_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/spi_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/sys_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/timer_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/us_ticker.c
SRC_C += $(VENDOR)/component/common/mbed/common/us_ticker_api.c
SRC_C += $(VENDOR)/component/common/mbed/common/wait_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/wdt_api.c

#peripheral - hal
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_32k.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_adc.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_gdma.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_gpio.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_i2c.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_i2s.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_mii.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_nfc.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_pcm.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_pwm.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_sdr_controller.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_ssi.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_timer.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_uart.c

#peripheral - osdep
SRC_C += $(VENDOR)/component/os/freertos/freertos_pmu.c

#peripheral - rtl8195a
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_adc.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_gdma.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_gpio.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_i2c.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_i2s.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_mii.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_nfc.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_pwm.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_ssi.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_timer.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_uart.c

#peripheral - wlan
#all:SRC_C += $(VENDOR)/component/common/drivers/wlan/realtek/src/core/option/rtw_opt_skbuf.c

#SDRAM
DRAM_C += $(VENDOR)/component/common/api/platform/stdlib_patch.c
#SDRAM - polarssl
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/aes.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/aesni.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/arc4.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/asn1parse.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/asn1write.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/base64.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/bignum.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/blowfish.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/camellia.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/ccm.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/certs.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/cipher.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/cipher_wrap.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/ctr_drbg.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/debug.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/des.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/dhm.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/ecp.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/ecp_curves.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/ecdh.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/ecdsa.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/entropy.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/entropy_poll.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/error.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/gcm.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/havege.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/hmac_drbg.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/md.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/md_wrap.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/md2.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/md4.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/md5.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/memory_buffer_alloc.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/net.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/oid.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/padlock.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/pbkdf2.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/pem.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/pkcs5.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/pkcs11.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/pkcs12.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/pk.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/pk_wrap.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/pkparse.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/pkwrite.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/platform.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/ripemd160.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/rsa.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/sha1.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/sha256.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/sha512.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/ssl_cache.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/ssl_ciphersuites.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/ssl_cli.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/ssl_srv.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/ssl_tls.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/threading.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/timing.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/version.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/version_features.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/x509.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/x509_crt.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/x509_crl.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/x509_csr.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/x509_create.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/x509write_crt.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/x509write_csr.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/xtea.c

#SDRAM - ssl_ram_map
DRAM_C += $(VENDOR)/component/common/network/ssl/ssl_ram_map/rom/rom_ssl_ram_map.c
DRAM_C += $(VENDOR)/component/common/network/ssl/ssl_ram_map/ssl_ram_map.c

#SDRAM - wigadget
#DRAM_C += $(VENDOR)/component/common/application/wigadget/cloud_link.c
#DRAM_C += $(VENDOR)/component/common/application/wigadget/shtc1.c
#DRAM_C += $(VENDOR)/component/common/application/wigadget/wigadget.c


#utilities
SRC_C += $(VENDOR)/component/common/utilities/cJSON.c
SRC_C += $(VENDOR)/component/common/utilities/http_client.c
SRC_C += $(VENDOR)/component/common/utilities/uart_socket.c
SRC_C += $(VENDOR)/component/common/utilities/webserver.c
SRC_C += $(VENDOR)/component/common/utilities/xml.c
#utilities - example
SRC_C += $(VENDOR)/component/common/example/example_entry.c
#SRC_C += $(VENDOR)/component/common/example/uart_atcmd/example_uart_atcmd.c

#utilities - FatFS
#SRC_C += $(VENDOR)/component/common/file_system/fatfs/fatfs_ext/src/ff_driver.c
#SRC_C += $(VENDOR)/component/common/file_system/fatfs/r0.10c/src/diskio.c
#SRC_C += $(VENDOR)/component/common/file_system/fatfs/r0.10c/src/ff.c
#SRC_C += $(VENDOR)/component/common/file_system/fatfs/r0.10c/src/option/ccsbcs.c
#SRC_C += $(VENDOR)/component/common/file_system/fatfs/disk_if/src/sdcard.c

#utilities - xmodme update
#SRC_C += $(VENDOR)/component/common/application/xmodem/uart_fw_update.c
#user 
SRC_C += main.c

# put micropython source to sdram section
PY_C += $(wildcard ../py/*.c)
PY_C += $(wildcard ../extmod/*.c)
PY_C += $(wildcard mphelper/*.c)
PY_C += $(wildcard mphelper/gccollect/*.c)
PY_C += mphelper/mods/modmachine.c
PY_C += mphelper/mods/moduwireless.c
PY_C += mphelper/mods/modnetwork.c
PY_C += mphelper/mods/modutime.c
PY_C += mphelper/mods/modterm.c
PY_C += mphelper/mods/moduos.c
PY_C += mphelper/mods/modusocket.c
PY_C += mphelper/mods/modussl.c
PY_C += mphelper/mods/modcrypto.c
PY_C += mphelper/mods/machine/objloguart.c
PY_C += mphelper/mods/machine/objsdio.c
PY_C += mphelper/mods/machine/objflash.c
PY_C += mphelper/mods/machine/objwdt.c
PY_C += mphelper/mods/machine/objrtc.c
PY_C += mphelper/mods/machine/objadc.c

PY_C += mphelper/mods/wireless/objwlan.c

PY_C += mphelper/mods/network/objnetif.c

PY_C := $(filter-out ../extmod/modlwip.c,$(PY_C))

PY_C += \
		lib/libc/string0.c \
		lib/utils/pyexec.c \
		lib/utils/pyhelp.c \
		lib/mp-readline/readline.c \
		lib/netutils/netutils.c \
		lib/utils/interrupt_char.c \
		lib/fatfs/ff.c \
		lib/fatfs/option/ccsbcs.c \
		lib/timeutils/timeutils.c \
		ftp/vfs.c \
		ftp/ftpd.c \

DRAM_C += $(PY_C)

ASM_S += $(wildcard mphelper/gccollect/*.s)

# Generate obj list
# -------------------------------------------------------------------

SRC_O = $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
#SRC_O = $(patsubst %.c,%.o,$(SRC_C))
DRAM_O = $(addprefix $(BUILD)/, $(DRAM_C:.c=.o))
#DRAM_O = $(patsubst %.c,%.o,$(DRAM_C))
ASM_O = $(addprefix $(BUILD)/, $(ASM_S:.s=.o))

OBJ = $(SRC_O) $(DRAM_O) $(ASM_O)

SRC_QSTR += $(PY_C)
SRC_QSTR_AUTO_DEPS +=


# Compile options
# -------------------------------------------------------------------

CFLAGS =
CFLAGS += -DM3 -DCONFIG_PLATFORM_8195A -DGCC_ARMCM3 -DARDUINO_SDK
CFLAGS += -mcpu=cortex-m3 -mthumb -g2 -w -O2 -Wno-pointer-sign -fno-common -fmessage-length=0  -ffunction-sections -fdata-sections -fomit-frame-pointer -fno-short-enums -mcpu=cortex-m3 -DF_CPU=166000000L -std=gnu99 -fsigned-char
CFLAGS += $(INCLUDES)

LFLAGS = 
LFLAGS += -mcpu=cortex-m3 -mthumb -g --specs=nano.specs -nostartfiles -Wl,-Map=$(BIN_DIR)/application.map -Os -Wl,--gc-sections -Wl,--cref -Wl,--entry=Reset_Handler -Wl,--no-enum-size-warning -Wl,--no-wchar-size-warning

LIBFLAGS =
all: LIBFLAGS += -L$(VENDOR)/component/soc/realtek/8195a/misc/bsp/lib/common/GCC/ -l_platform -l_wlan -l_p2p -l_wps -l_rtlstd -l_websocket -l_xmodem -l_sdcard -lm -lc -lnosys -lgcc
mp: LIBFLAGS += -L$(VENDOR)/component/soc/realtek/8195a/misc/bsp/lib/common/GCC/ -l_platform -l_wlan_mp -l_p2p -l_wps -l_rtlstd -l_websocket -l_xmodem -l_sdcard -lm -lc -lnosys -lgcc

RAMALL_BIN =
OTA_BIN = 
all: RAMALL_BIN = ram_all.bin
all: OTA_BIN = ota.bin
mp: RAMALL_BIN = ram_all_mp.bin
mp: OTA_BIN = ota_mp.bin


# Compile
# -------------------------------------------------------------------

application: prerequirement build_info $(SRC_O) $(DRAM_O) $(ASM_O)
	$(LD) $(LFLAGS) -o $(BIN_DIR)/$(TARGET).axf  $(OBJ) $(OBJ_DIR)/ram_1.r.o $(LIBFLAGS) -L$(TOOL) -T$(TOOL)/rlx8195A-symbol-v02-img2.ld 
	$(OBJDUMP) -d $(BIN_DIR)/$(TARGET).axf > $(BIN_DIR)/$(TARGET).asm


# Manipulate Image
# -------------------------------------------------------------------
	
.PHONY: manipulate_images
manipulate_images:	
	@echo ===========================================================
	@echo Image manipulating
	@echo ===========================================================
	$(NM) $(BIN_DIR)/$(TARGET).axf | sort > $(BIN_DIR)/$(TARGET).nmap
	$(OBJCOPY) -j .image2.start.table -j .ram_image2.text -j .ram_image2.rodata -j .ram.data -Obinary $(BIN_DIR)/$(TARGET).axf $(BIN_DIR)/ram_2.bin
	$(OBJCOPY) -j .sdr_text -j .sdr_rodata -j .sdr_data -Obinary $(BIN_DIR)/$(TARGET).axf $(BIN_DIR)/sdram.bin
	cp $(VENDOR)/component/soc/realtek/8195a/misc/bsp/image/ram_1.p.bin $(BIN_DIR)/ram_1.p.bin
	chmod 777 $(BIN_DIR)/ram_1.p.bin
	chmod +rx $(PICK) $(CHKSUM) $(PAD)
	$(PICK) 0x`grep __ram_image2_text_start__ $(BIN_DIR)/$(TARGET).nmap | gawk '{print $$1}'` 0x`grep __ram_image2_text_end__ $(BIN_DIR)/$(TARGET).nmap | gawk '{print $$1}'` $(BIN_DIR)/ram_2.bin $(BIN_DIR)/ram_2.p.bin body+reset_offset+sig
	$(PICK) 0x`grep __ram_image2_text_start__ $(BIN_DIR)/$(TARGET).nmap | gawk '{print $$1}'` 0x`grep __ram_image2_text_end__ $(BIN_DIR)/$(TARGET).nmap | gawk '{print $$1}'` $(BIN_DIR)/ram_2.bin $(BIN_DIR)/ram_2.ns.bin body+reset_offset
	$(PICK) 0x`grep __sdram_data_start__ $(BIN_DIR)/$(TARGET).nmap | gawk '{print $$1}'` 0x`grep __sdram_data_end__ $(BIN_DIR)/$(TARGET).nmap | gawk '{print $$1}'` $(BIN_DIR)/sdram.bin $(BIN_DIR)/ram_3.p.bin body+reset_offset
	$(PAD) 44k 0xFF $(BIN_DIR)/ram_1.p.bin
	cat $(BIN_DIR)/ram_1.p.bin > $(BIN_DIR)/$(RAMALL_BIN)
	chmod 777 $(BIN_DIR)/$(RAMALL_BIN)
	cat $(BIN_DIR)/ram_2.p.bin >> $(BIN_DIR)/$(RAMALL_BIN)
	if [ -s $(BIN_DIR)/sdram.bin ]; then cat $(BIN_DIR)/ram_3.p.bin >> $(BIN_DIR)/$(RAMALL_BIN); fi
	cat $(BIN_DIR)/ram_2.ns.bin > $(BIN_DIR)/$(OTA_BIN)
	chmod 777 $(BIN_DIR)/$(OTA_BIN)
	if [ -s $(BIN_DIR)/sdram.bin ]; then cat $(BIN_DIR)/ram_3.p.bin >> $(BIN_DIR)/$(OTA_BIN); fi
	$(CHKSUM) $(BIN_DIR)/$(OTA_BIN) || true
	rm $(BIN_DIR)/ram_*.p.bin $(BIN_DIR)/ram_*.ns.bin

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
	@echo ===========================================================
	@echo Build $(TARGET)
	@echo ===========================================================
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)
	cp $(VENDOR)/component/soc/realtek/8195a/misc/bsp/image/ram_1.r.bin $(OBJ_DIR)/ram_1.r.bin
	chmod 777 $(OBJ_DIR)/ram_1.r.bin
	$(OBJCOPY) --rename-section .data=.loader.data,contents,alloc,load,readonly,data -I binary -O elf32-littlearm -B arm $(OBJ_DIR)/ram_1.r.bin $(OBJ_DIR)/ram_1.r.o 

$(SRC_O): $(BUILD)/%.o : %.c
	$(MKDIR) -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) $(CFLAGS) -c $< -MM -MT $@ -MF $(OBJ_DIR)/$(notdir $(patsubst %.o,%.d,$@))
	#cp $@ $(OBJ_DIR)/$(notdir $@)
	#chmod 777 $(OBJ_DIR)/$(notdir $@)

$(DRAM_O): $(BUILD)/%.o : %.c 
	$(MKDIR) -p $(dir $@)
	$(CC) $(CFLAGS)  -c $< -o $@
	$(OBJCOPY) --prefix-alloc-sections .sdram $@
	$(CC) $(CFLAGS)  -c $< -MM -MT $@ -MF $(OBJ_DIR)/$(notdir $(patsubst %.o,%.d,$@))
	#cp $@ $(OBJ_DIR)/$(notdir $@)
	#chmod 777 $(OBJ_DIR)/$(notdir $@)

$(ASM_O): $(BUILD)/%.o : %.s
	$(MKDIR) -p $(dir $@)
	$(CC) $(CFLAGS)  -c $< -o $@
	$(OBJCOPY) --prefix-alloc-sections .sdram $@
	$(CC) $(CFLAGS)  -c $< -MM -MT $@ -MF $(OBJ_DIR)/$(notdir $(patsubst %.o,%.d,$@))
	#cp $@ $(OBJ_DIR)/$(notdir $@)
	#chmod 777 $(OBJ_DIR)/$(notdir $@)

# Generate build info
# -------------------------------------------------------------------	
#ifeq (setup,$(firstword $(MAKECMDGOALS)))
#  # use the rest as arguments for "run"
#  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
#  # ...and turn them into do-nothing targets
#  $(eval $(RUN_ARGS):;@:)
#endif
.PHONY: setup
setup:
	@echo "----------------"
	@echo Setup $(GDB_SERVER)
	@echo "----------------"
ifeq ($(GDB_SERVER), openocd)
	cp -p $(FLASH_TOOLDIR)/rtl_gdb_debug_openocd.txt $(FLASH_TOOLDIR)/rtl_gdb_debug.txt
	cp -p $(FLASH_TOOLDIR)/rtl_gdb_ramdebug_openocd.txt $(FLASH_TOOLDIR)/rtl_gdb_ramdebug.txt
	cp -p $(FLASH_TOOLDIR)/rtl_gdb_flash_write_openocd.txt $(FLASH_TOOLDIR)/rtl_gdb_flash_write.txt
else
	cp -p $(FLASH_TOOLDIR)/rtl_gdb_debug_jlink.txt $(FLASH_TOOLDIR)/rtl_gdb_debug.txt
	cp -p $(FLASH_TOOLDIR)/rtl_gdb_ramdebug_jlink.txt $(FLASH_TOOLDIR)/rtl_gdb_ramdebug.txt
	cp -p $(FLASH_TOOLDIR)/rtl_gdb_flash_write_jlink.txt $(FLASH_TOOLDIR)/rtl_gdb_flash_write.txt
endif

.PHONY: flashburn
flashburn:
	@if [ ! -f $(FLASH_TOOLDIR)/rtl_gdb_flash_write.txt ] ; then echo Please do \"make setup GDB_SERVER=[jlink or openocd]\" first; echo && false ; fi
ifeq ($(findstring CYGWIN, $(OS)), CYGWIN) 
	$(FLASH_TOOLDIR)/Check_Jtag.sh
endif
	cp	$(FLASH_TOOLDIR)/target_NORMALB.axf $(FLASH_TOOLDIR)/target_NORMAL.axf
	chmod 777 $(FLASH_TOOLDIR)/target_NORMAL.axf
	chmod +rx $(FLASH_TOOLDIR)/SetupGDB_NORMAL.sh
	$(FLASH_TOOLDIR)/SetupGDB_NORMAL.sh
	$(GDB) -x $(FLASH_TOOLDIR)/rtl_gdb_flash_write.txt
	
.PHONY: debug
debug:
	@if [ ! -f $(FLASH_TOOLDIR)/rtl_gdb_debug.txt ] ; then echo Please do \"make setup GDB_SERVER=[jlink or openocd]\" first; echo && false ; fi
ifeq ($(findstring CYGWIN, $(OS)), CYGWIN) 
	$(FLASH_TOOLDIR)/Check_Jtag.sh
	cmd /c start $(GDB) -x $(FLASH_TOOLDIR)/rtl_gdb_debug.txt
else
	$(GDB) -x $(FLASH_TOOLDIR)/rtl_gdb_debug.txt
endif

.PHONY: ramdebug
ramdebug:
	@if [ ! -f $(FLASH_TOOLDIR)/rtl_gdb_ramdebug.txt ] ; then echo Please do \"make setup GDB_SERVER=[jlink or openocd]\" first; echo && false ; fi
ifeq ($(findstring CYGWIN, $(OS)), CYGWIN) 
	$(FLASH_TOOLDIR)/Check_Jtag.sh
	cmd /c start $(GDB) -x $(FLASH_TOOLDIR)/rtl_gdb_ramdebug.txt	
else
	$(GDB) -x $(FLASH_TOOLDIR)/rtl_gdb_ramdebug.txt	
endif

.PHONY: clean
distclean:
	rm -rf $(TARGET)
	rm -f $(SRC_O) $(DRAM_O)
	rm -f $(patsubst %.o,%.d,$(SRC_O)) $(patsubst %.o,%.d,$(DRAM_O))
	
.PHONY: clean_all
clean_all:
	rm -rf $(ARM_GCC_TOOLCHAIN)
	rm -rf $(TARGET)
	rm -f $(SRC_O) $(DRAM_O)
	rm -f $(patsubst %.o,%.d,$(SRC_O)) $(patsubst %.o,%.d,$(DRAM_O))

print:
	$(ECHO) $(OBJ)
	
include ../py/mkrules.mk
