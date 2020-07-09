ifeq ($(findstring CYGWIN, $(OS)), CYGWIN) 
PICK = $(BUILD)pick.exe
PAD  = $(BUILD)padding.exe
CHKSUM = $(BUILD)checksum.exe
else
ifeq ($(findstring Darwin, $(OS)), Darwin) 
PICK = $(BUILD)/pick
PAD  = $(BUILD)/padding
CHKSUM = $(BUILD)/checksum
AMEBASIZE = $(BUILD)/amebasize
else
PICK = $(BUILD)/pick
PAD  = $(BUILD)/padding
CHKSUM = $(BUILD)/checksum
AMEBASIZE = $(BUILD)/amebasize
endif
endif

INC += -I.
INC += -Iinc/ameba
INC += -I$(TOP)
INC += -I$(BUILD)
INC += -I$(HEADER_BUILD)
INC += -I$(VENDOR)/component/soc/realtek/common/bsp
INC += -I$(VENDOR)/component/os/freertos
INC += -I$(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/include
INC += -I$(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/portable/GCC/ARM_CM3
INC += -I$(VENDOR)/component/os/os_dep/include
INC += -I$(VENDOR)/component/soc/realtek/8195a/misc/driver
INC += -I$(VENDOR)/component/soc/realtek/8195a/misc/os
INC += -I$(VENDOR)/component/common/api/network/include
INC += -I$(VENDOR)/component/common/api
INC += -I$(VENDOR)/component/common/api/platform
INC += -I$(VENDOR)/component/common/api/wifi
INC += -I$(VENDOR)/component/common/api/wifi/rtw_wpa_supplicant/src

INC += -I$(VENDOR)/component/common/media/framework
INC += -I$(VENDOR)/component/common/example
INC += -I$(VENDOR)/component/common/example/wlan_fast_connect
INC += -I$(VENDOR)/component/common/mbed/api
INC += -I$(VENDOR)/component/common/mbed/hal
INC += -I$(VENDOR)/component/common/mbed/hal_ext
INC += -I$(VENDOR)/component/common/mbed/targets/hal/rtl8195a
INC += -I$(VENDOR)/component/common/network
INC += -I$(VENDOR)/component/common/network/lwip/lwip_v1.4.1/port/realtek/freertos
INC += -I$(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/include
INC += -I$(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/include/lwip
INC += -I$(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/include/ipv4
INC += -I$(VENDOR)/component/common/network/lwip/lwip_v1.4.1/port/realtek
INC += -I$(VENDOR)/component/common/test
INC += -I$(VENDOR)/component/soc/realtek/8195a/cmsis
INC += -I$(VENDOR)/component/soc/realtek/8195a/cmsis/device
INC += -I$(VENDOR)/component/soc/realtek/8195a/fwlib
INC += -I$(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a
INC += -I$(VENDOR)/component/soc/realtek/8195a/misc/rtl_std_lib/include
INC += -I$(VENDOR)/component/common/drivers/wlan/realtek/include
INC += -I$(VENDOR)/component/common/drivers/wlan/realtek/src/osdep
INC += -I$(VENDOR)/component/common/drivers/wlan/realtek/src/hci
INC += -I$(VENDOR)/component/common/drivers/wlan/realtek/src/hal
INC += -I$(VENDOR)/component/common/drivers/wlan/realtek/src/hal/OUTSRC
INC += -I$(VENDOR)/component/soc/realtek/8195a/fwlib/ram_lib/wlan/realtek/wlan_ram_map/rom
INC += -I$(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/include
INC += -I$(VENDOR)/component/common/network/ssl/ssl_ram_map/rom
INC += -I$(VENDOR)/component/common/utilities
INC += -I$(VENDOR)/component/soc/realtek/8195a/misc/rtl_std_lib/include

INC += -I$(VENDOR)/component/soc/realtek/8195a/fwlib/ram_lib/usb_otg/include
INC += -I$(VENDOR)/component/common/video/v4l2/inc
INC += -I$(VENDOR)/component/common/media/codec
INC += -I$(VENDOR)/component/common/drivers/usb_class/host/uvc/inc
INC += -I$(VENDOR)/component/common/drivers/usb_class/device
INC += -I$(VENDOR)/component/common/drivers/usb_class/device/class

INC += -I$(VENDOR)/component/common/drivers/sdio/realtek/sdio_host/inc
INC += -I$(VENDOR)/component/common/audio
INC += -I$(VENDOR)/component/common/drivers/i2s

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

SRC_C =
DRAM_C =

SRC_C += $(VENDOR)/component/soc/realtek/8195a/cmsis/device/system_8195a.c

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
#SRC_C += $(VENDOR)/component/common/network/dhcp/dhcps.c
#SRC_C += $(VENDOR)/component/common/network/sntp/sntp.c

#network - mdns
SRC_C += $(VENDOR)/component/common/network/mDNS/mDNSPlatform.c


#os - freertos
SRC_C += $(VENDOR)/component/os/freertos/cmsis_os.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/portable/MemMang/heap_4.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/portable/GCC/ARM_CM3/port.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/croutine.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/event_groups.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/list.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/queue.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/tasks.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/timers.c

#os - osdep
SRC_C += $(VENDOR)/component/os/os_dep/device_lock.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_service.c
SRC_C += $(VENDOR)/component/os/os_dep/osdep_service.c
SRC_C += $(VENDOR)/component/os/os_dep/tcm_heap.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/misc/os/osdep_api.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/misc/os/freertos_pmu_8195a.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/misc/os/mailbox.c

#peripheral - api
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/analogin_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/dma_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/efuse_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/ethernet_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/flash_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/gpio_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/gpio_irq_api.c

# i2c_api.c is a special file that it has so many bss, so put it in SDRAM
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/i2c_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/serial_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/i2s_api.c
DRAM_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/log_uart_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/pinmap.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/pinmap_common.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/port_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/rtc_api.c
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
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_gdma.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_gpio.c

# hal_i2c.c is a special file that it has so many bss, so put it in SDRAM
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_i2c.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_uart.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_i2s.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_sdr_controller.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_ssi.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_timer.c


#peripheral - osdep
#SRC_C += $(VENDOR)/component/os/freertos/freertos_pmu.c

#peripheral - rtl8195a
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_gdma.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_gpio.c

# rtl8195a_i2c.c is a special file that it has so many bss, so put it in SDRAM
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_i2c.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_uart.c

SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_i2s.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_ssi.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_timer.c

SRC_C += $(VENDOR)/component/common/api/platform/stdlib_patch.c


#SDRAM - peri of RTL8195A only , so thay can be put in SDRAM memory
DRAM_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_adc.c
DRAM_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_dac.c
DRAM_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_nfc.c
DRAM_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_mii.c
DRAM_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/rtl8195a/src/rtl8195a_pwm.c

DRAM_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_adc.c
DRAM_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_dac.c
DRAM_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_nfc.c
DRAM_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_mii.c
DRAM_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_pwm.c
DRAM_C += $(VENDOR)/component/soc/realtek/8195a/fwlib/src/hal_pcm.c

DRAM_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/nfc_api.c
DRAM_C += $(VENDOR)/component/common/drivers/ethernet_mii/ethernet_mii.c
DRAM_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/pwmout_api.c

#SDRAM - polarssl
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/aes.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/aesni.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/arc4.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/asn1parse.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/asn1write.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/base64.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/bignum.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/blowfish.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/camellia.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/ccm.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/certs.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/cipher.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/cipher_wrap.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/ctr_drbg.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/debug.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/des.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/dhm.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/ecp.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/ecp_curves.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/ecdh.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/ecdsa.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/entropy.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/entropy_poll.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/error.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/gcm.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/havege.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/hmac_drbg.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/md.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/md_wrap.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/md2.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/md4.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/md5.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/memory_buffer_alloc.c
#DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/net.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/oid.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/padlock.c
#DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/pbkdf2.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/pem.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/pkcs5.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/pkcs11.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/pkcs12.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/pk.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/pk_wrap.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/pkparse.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/pkwrite.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/platform.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/ripemd160.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/rsa.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/sha1.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/sha256.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/sha512.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/ssl_cache.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/ssl_ciphersuites.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/ssl_cli.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/ssl_srv.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/ssl_tls.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/threading.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/timing.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/version.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/version_features.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/x509.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/x509_crt.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/x509_crl.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/x509_csr.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/x509_create.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/x509write_crt.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/x509write_csr.c
DRAM_C += $(VENDOR)/component/common/network/ssl/mbedtls-2.4.0/library/xtea.c

#SDRAM - ssl_ram_map
DRAM_C += $(VENDOR)/component/common/network/ssl/ssl_ram_map/rom/rom_ssl_ram_map.c
DRAM_C += $(VENDOR)/component/common/network/ssl/ssl_ram_map/ssl_ram_map.c

# put micropython source to sdram section
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

UPY_C += main.c

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

DRAM_C += $(UPY_C)

# Initialize target name and target object files
# -------------------------------------------------------------------

all: application manipulate_images

TARGET=application

# Include folder list
# -------------------------------------------------------------------

# Generate obj list
# -------------------------------------------------------------------

SRC_O = $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
DRAM_O = $(addprefix $(BUILD)/, $(DRAM_C:.c=.o))

OBJ = $(PY_O) $(SRC_O) $(DRAM_O)
SRC_QSTR += $(UPY_C)
SRC_QSTR_AUTO_DEPS +=

################################
# 			CFLAGS 			   #
################################

# Optimize level
CFLAGS = -O2

# CPU arch
CFLAGS += -mcpu=cortex-m3 -mthumb

ifeq ($(DEBUG), 1)
CFLAGS += -g3 -ggdb
else 
CFLAGS += -g2
endif

# source code macro
CFLAGS += -DM3 -DCONFIG_PLATFORM_8195A -DGCC_ARMCM3 -D$(CHIP) -DF_CPU=166000000L
CFLAGS += -DMBEDTLS_CONFIG_FILE=\"mbedtls-config.h\"

CFLAGS += -w -Wno-pointer-sign -fno-common -fmessage-length=0  -ffunction-sections -fdata-sections -fomit-frame-pointer -fno-short-enums -std=gnu99 -fsigned-char
CFLAGS += $(CFLAGS_MOD)
CFLAGS += $(INC)

LFLAGS =
LFLAGS += -mcpu=cortex-m3 -mthumb -g --specs=nano.specs -nostartfiles -Wl,-Map=$(BUILD)/application.map -Wl,--gc-sections -Wl,--cref -Wl,--entry=Reset_Handler -Wl,--no-enum-size-warning -Wl,--no-wchar-size-warning

LIBFLAGS += -L$(VENDOR)/component/soc/realtek/8195a/misc/bsp/lib/common/GCC/ -l_platform -l_wlan -l_wps -l_rtlstd -l_websocket -l_xmodem -lm -lc -lnosys -lgcc

RAMALL_BIN = ram_all.bin

application: prerequirement build_info $(SRC_O) $(DRAM_O) $(PY_O)
	$(ECHO) "Building $(CHIP) "
	$(Q)$(LD) $(LFLAGS) -o $(BUILD)/$(TARGET).axf $(OBJ) $(BUILD)/ram_1.r.o $(LIBFLAGS) -L$(TOOL)/ameba1 -l_analout_api -T$(TOOL)/ameba1/$(CHIP)-symbol-v02.ld 
	$(Q)$(OBJDUMP) -d $(BUILD)/$(TARGET).axf > $(BUILD)/$(TARGET).asm

.PHONY: manipulate_images
manipulate_images: $(PICK) $(PAD) $(CHKSUM) $(AMEBASIZE)
	$(Q)echo ===========================================================
	$(Q)echo Image manipulating
	$(Q)echo ===========================================================
	$(Q)$(NM) $(BUILD)/$(TARGET).axf | sort > $(BUILD)/$(TARGET).nmap
	$(Q)$(OBJCOPY) -j .ram.start.table -j .ram.text -j .ram.rodata -j .ram.data -Obinary $(BUILD)/$(TARGET).axf $(BUILD)/ram_2.bin
	$(Q)$(OBJCOPY) -j .sdram.text -j .sdram.rodata -j .sdram.data -Obinary $(BUILD)/$(TARGET).axf $(BUILD)/sdram.bin
	$(Q)cp -f $(TOOL)/ameba1/image/ram_1.p.bin $(BUILD)/ram_1.p.bin
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

	@echo \#define RTL195AFW_COMPILER \"gcc `$(CC) $(CFLAGS) -dumpversion `\" >> .ver
	@mv -f .ver inc/ameba/$@.h


.PHONY: prerequirement
prerequirement:
	$(Q)echo ===========================================================
	$(Q)echo Build $(TARGET)
	$(Q)echo ===========================================================
	$(Q)$(MKDIR) -p $(BUILD)
	$(Q)cp -f $(TOOL)/ameba1/image/ram_1.r.bin $(BUILD)/ram_1.r.bin
	$(Q)chmod +rw $(BUILD)/ram_1.r.bin
	$(Q)$(OBJCOPY) --rename-section .data=.loader.data,contents,alloc,load,readonly,data -I binary -O elf32-littlearm -B arm $(BUILD)/ram_1.r.bin $(BUILD)/ram_1.r.o 

$(PICK): tool/ameba1/src/$(OS)/pick.c
	$(Q)gcc -o $@ $<

$(PAD): tool/ameba1/src/$(OS)/padding.c
	$(Q)gcc -o $@ $<

$(CHKSUM): tool/ameba1/src/$(OS)/checksum.c
	$(Q)gcc -o $@ $< 

$(AMEBASIZE): tool/ameba1/src/$(OS)/amebasize.c
	$(Q)gcc -o $@ $<

$(SRC_O): $(BUILD)/%.o : %.c
	$(Q)$(MKDIR) -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(DRAM_O): $(BUILD)/%.o : %.c 
	$(Q)$(MKDIR) -p $(dir $@)
	$(CC) $(CFLAGS)  -c $< -o $@
	$(Q)$(OBJCOPY) --prefix-alloc-sections .sdram $@

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