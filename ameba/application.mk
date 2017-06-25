INC += -I.
INC += -I..
INC += -Iinc
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
INC += -I$(VENDOR)/component/common/network/ssl/polarssl-1.3.8/include
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

INC += -I../extmod
INC += -I../lib/utils
INC += -I../lib/timeutils
INC += -I../lib/mp-readline
INC += -I../lib/netutils

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
#SRC_C += $(VENDOR)/component/soc/realtek/8195a/misc/driver/rtl_consol.c

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
SRC_C += $(VENDOR)/component/common/network/dhcp/dhcps.c
SRC_C += $(VENDOR)/component/common/network/sntp/sntp.c

#network - mdns
SRC_C += $(VENDOR)/component/common/network/mDNS/mDNSPlatform.c


#os - freertos
SRC_C += $(VENDOR)/component/os/freertos/cmsis_os.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/portable/MemMang/heap_5.c
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
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8195a/log_uart_api.c
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
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/aes.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/aesni.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/arc4.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/asn1parse.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/asn1write.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/base64.c
DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/bignum.c
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
#DRAM_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/net.c
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

# put micropython source to sdram section
DRAM_C += pins.c
DRAM_C += mphelper/diskio.c
DRAM_C += mphelper/exception.c
DRAM_C += mphelper/help.c
DRAM_C += mphelper/mphal.c
DRAM_C += mphelper/input.c
DRAM_C += mphelper/bufhelper.c
DRAM_C += mphelper/mpthreadport.c
DRAM_C += mphelper/gccollect/gccollect.c
DRAM_C += mphelper/mods/modameba.c
DRAM_C += mphelper/mods/modmachine.c
DRAM_C += mphelper/mods/modlwip.c
DRAM_C += mphelper/mods/moduwireless.c
DRAM_C += mphelper/mods/modnetwork.c
DRAM_C += mphelper/mods/modutime.c
DRAM_C += mphelper/mods/modterm.c
DRAM_C += mphelper/mods/moduos.c
DRAM_C += mphelper/mods/modussl.c
DRAM_C += mphelper/mods/machine/objloguart.c
#DRAM_C += mphelper/mods/machine/objsdio_host.c
DRAM_C += mphelper/mods/machine/objwdt.c
DRAM_C += mphelper/mods/machine/objrtc.c
DRAM_C += mphelper/mods/machine/objadc.c
DRAM_C += mphelper/mods/machine/objdac.c
DRAM_C += mphelper/mods/machine/objpin.c
DRAM_C += mphelper/mods/machine/obji2c.c
DRAM_C += mphelper/mods/machine/objpwm.c
DRAM_C += mphelper/mods/machine/objtimer.c
DRAM_C += mphelper/mods/machine/objspi.c
DRAM_C += mphelper/mods/machine/objuart.c
DRAM_C += mphelper/mods/machine/objcrypto.c

DRAM_C += mphelper/mods/wireless/objwlan.c
DRAM_C += mphelper/mods/network/objnetif.c

DRAM_C += main.c
SRC_C += dummy.c

DRAM_C += \
		lib/utils/pyexec.c \
		lib/mp-readline/readline.c \
		lib/utils/interrupt_char.c \
		lib/oofatfs/ff.c \
		lib/oofatfs/option/ccsbcs.c \
		lib/timeutils/timeutils.c \
		lib/netutils/netutils.c \
		lib//utils/sys_stdio_mphal.c \
