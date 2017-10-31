AMEBA_TOOLDIR = ./tool/amebaz/
FLASH_TOOLDIR = $(VENDOR)/component/soc/realtek/8195a/misc/gcc_utility/
FLASHDOWNLOAD_TOOLDIR = $(VENDOR)/component/soc/realtek/8711b/misc/gnu_utility/flash_download/image
DEBUG_TOOLDIR = $(VENDOR)/component/soc/realtek/8711b/misc/gcc_utility/

ifeq ($(findstring CYGWIN, $(OS)), CYGWIN) 
PICK = $(AMEBA_TOOLDIR)pick.exe
PAD  = $(AMEBA_TOOLDIR)padding.exe
CHKSUM = $(AMEBA_TOOLDIR)checksum.exe
else
ifeq ($(findstring Darwin, $(OS)), Darwin) 
PICK = $(TOOL)/mac/pick
PAD  = $(TOOL)/mac/padding
CHKSUM = $(TOOL)/mac/checksum
else
PICK = $(AMEBA_TOOLDIR)pick
PAD  = $(AMEBA_TOOLDIR)padding
CHKSUM = $(AMEBA_TOOLDIR)checksum
endif
endif

INC =
INC += -I.
INC += -Iinc/amebaz
INC += -I$(TOP)
INC += -I$(BUILD)
INC += -I$(HEADER_BUILD)
INC += -I$(VENDOR)/component/os/freertos
INC += -I$(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/include
INC += -I$(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/portable/GCC/ARM_CM4F
INC += -I$(VENDOR)/component/os/os_dep/include
INC += -I$(VENDOR)/component/common/api/network/include
INC += -I$(VENDOR)/component/common/api
INC += -I$(VENDOR)/component/common/api/at_cmd
INC += -I$(VENDOR)/component/common/api/platform
INC += -I$(VENDOR)/component/common/api/wifi
INC += -I$(VENDOR)/component/common/api/wifi/rtw_wpa_supplicant/src
INC += -I$(VENDOR)/component/common/api/wifi/rtw_wowlan
INC += -I$(VENDOR)/component/common/api/wifi/rtw_wpa_supplicant/wpa_supplicant
INC += -I$(VENDOR)/component/common/application
INC += -I$(VENDOR)/component/common/application/mqtt/MQTTClient
INC += -I$(VENDOR)/component/common/application/mqtt/MQTTPacket
INC += -I$(VENDOR)/component/common/example
INC += -I$(VENDOR)/component/common/example/wlan_fast_connect
INC += -I$(VENDOR)/component/common/drivers/modules
INC += -I$(VENDOR)/component/common/drivers/sdio/realtek/sdio_host/inc
INC += -I$(VENDOR)/component/common/drivers/inic/rtl8711b
INC += -I$(VENDOR)/component/common/drivers/usb_class/device
INC += -I$(VENDOR)/component/common/drivers/usb_class/device/class
INC += -I$(VENDOR)/component/common/drivers/wlan/realtek/include
INC += -I$(VENDOR)/component/common/drivers/wlan/realtek/src/osdep
INC += -I$(VENDOR)/component/common/drivers/wlan/realtek/src/hci
INC += -I$(VENDOR)/component/common/drivers/wlan/realtek/src/hal
INC += -I$(VENDOR)/component/common/drivers/wlan/realtek/src/hal/rtl8711b
INC += -I$(VENDOR)/component/common/drivers/wlan/realtek/src/hal/OUTSRC
INC += -I$(VENDOR)/component/common/drivers/wlan/realtek/wlan_ram_map/rom
INC += -I$(VENDOR)/component/common/file_system
INC += -I$(VENDOR)/component/common/network
INC += -I$(VENDOR)/component/common/network/lwip/lwip_v1.4.1/port/realtek/freertos
INC += -I$(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/include
INC += -I$(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/include/lwip
INC += -I$(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/include/ipv4
INC += -I$(VENDOR)/component/common/network/lwip/lwip_v1.4.1/port/realtek
INC += -I$(VENDOR)/component/common/network/ssl/polarssl-1.3.8/include
INC += -I$(VENDOR)/component/common/network/ssl/ssl_ram_map/rom
INC += -I$(VENDOR)/component/common/test
INC += -I$(VENDOR)/component/common/utilities
INC += -I$(VENDOR)/component/soc/realtek/8711b/app/monitor/include
INC += -I$(VENDOR)/component/soc/realtek/8711b/cmsis
INC += -I$(VENDOR)/component/soc/realtek/8711b/cmsis/device
INC += -I$(VENDOR)/component/soc/realtek/8711b/fwlib
INC += -I$(VENDOR)/component/soc/realtek/8711b/fwlib/include
INC += -I$(VENDOR)/component/soc/realtek/8711b/fwlib/ram_lib/crypto
INC += -I$(VENDOR)/component/soc/realtek/8711b/fwlib/rom_lib
INC += -I$(VENDOR)/component/soc/realtek/8711b/swlib/os_dep/include
INC += -I$(VENDOR)/component/soc/realtek/8711b/swlib/std_lib/include
INC += -I$(VENDOR)/component/soc/realtek/8711b/swlib/std_lib/libc/include
INC += -I$(VENDOR)/component/soc/realtek/8711b/swlib/std_lib/libc/rom/string
INC += -I$(VENDOR)/component/soc/realtek/8711b/swlib/std_lib/libgcc/rtl8195a/include
INC += -I$(VENDOR)/component/soc/realtek/8711b/swlib/rtl_lib
INC += -I$(VENDOR)/component/soc/realtek/8711b/misc
INC += -I$(VENDOR)/component/soc/realtek/8711b/misc/os
INC += -I$(VENDOR)/component/common/mbed/api
INC += -I$(VENDOR)/component/common/mbed/hal
INC += -I$(VENDOR)/component/common/mbed/hal_ext
INC += -I$(VENDOR)/component/common/mbed/targets/cmsis/rtl8711b
INC += -I$(VENDOR)/component/common/mbed/targets/hal/rtl8711b
INC += -I$(VENDOR)/project/realtek_8195a_gen_project/rtl8195a/sw/lib/sw_lib/mbed/api
INC += -I$(VENDOR)/component/common/application/mqtt/MQTTClient
INC += -I$(VENDOR)/component/common/network/websocket


# Source file list
# -------------------------------------------------------------------

SRC_C =

#app uart_adapter
#SRC_C += $(VENDOR)/component/common/application/uart_adapter/uart_adapter.c
      
#cmsis
SRC_C += $(VENDOR)/component/soc/realtek/8711b/cmsis/device/app_start.c
SRC_C += $(VENDOR)/component/soc/realtek/8711b/fwlib/ram_lib/startup.c
SRC_C += $(VENDOR)/component/soc/realtek/8711b/cmsis/device/system_8195a.c
    

#console
#SRC_C += $(VENDOR)/component/common/api/at_cmd/atcmd_lwip.c
#SRC_C += $(VENDOR)/component/common/api/at_cmd/atcmd_sys.c
SRC_C += $(VENDOR)/component/common/api/at_cmd/atcmd_wifi.c
SRC_C += $(VENDOR)/component/common/api/at_cmd/log_service.c
SRC_C += $(VENDOR)/component/soc/realtek/8711b/app/monitor/ram/low_level_io.c
SRC_C += $(VENDOR)/component/soc/realtek/8711b/app/monitor/ram/monitor.c
SRC_C += $(VENDOR)/component/soc/realtek/8711b/app/monitor/ram/rtl_consol.c
SRC_C += $(VENDOR)/component/soc/realtek/8711b/app/monitor/ram/rtl_trace.c
    
  
#network api wifi rtw_wpa_supplicant
SRC_C += $(VENDOR)/component/common/api/wifi/rtw_wpa_supplicant/wpa_supplicant/wifi_eap_config.c
SRC_C += $(VENDOR)/component/common/api/wifi/rtw_wpa_supplicant/wpa_supplicant/wifi_wps_config.c
          
        
#network api wifi        
SRC_C += $(VENDOR)/component/common/api/wifi/wifi_conf.c
SRC_C += $(VENDOR)/component/common/api/wifi/wifi_ind.c
SRC_C += $(VENDOR)/component/common/api/wifi/wifi_promisc.c
#SRC_C += $(VENDOR)/component/common/api/wifi/wifi_simple_config.c
SRC_C += $(VENDOR)/component/common/api/wifi/wifi_util.c
        
      
#network api      
SRC_C += $(VENDOR)/component/common/api/lwip_netconf.c
      
    
#network app
#SRC_C += $(VENDOR)/component/common/application/mqtt/MQTTClient/MQTTClient.c
#SRC_C += $(VENDOR)/component/common/application/mqtt/MQTTPacket/MQTTConnectClient.c
#SRC_C += $(VENDOR)/component/common/application/mqtt/MQTTPacket/MQTTConnectServer.c
#SRC_C += $(VENDOR)/component/common/application/mqtt/MQTTPacket/MQTTDeserializePublish.c
#SRC_C += $(VENDOR)/component/common/application/mqtt/MQTTPacket/MQTTFormat.c
#SRC_C += $(VENDOR)/component/common/application/mqtt/MQTTClient/MQTTFreertos.c
#SRC_C += $(VENDOR)/component/common/application/mqtt/MQTTPacket/MQTTPacket.c
#SRC_C += $(VENDOR)/component/common/application/mqtt/MQTTPacket/MQTTSerializePublish.c
#SRC_C += $(VENDOR)/component/common/application/mqtt/MQTTPacket/MQTTSubscribeClient.c
#SRC_C += $(VENDOR)/component/common/application/mqtt/MQTTPacket/MQTTSubscribeServer.c
#SRC_C += $(VENDOR)/component/common/application/mqtt/MQTTPacket/MQTTUnsubscribeClient.c
#SRC_C += $(VENDOR)/component/common/application/mqtt/MQTTPacket/MQTTUnsubscribeServer.c
#SRC_C += $(VENDOR)/component/common/api/network/src/ping_test.c
#SRC_C += $(VENDOR)/component/common/utilities/ssl_client.c
#SRC_C += $(VENDOR)/component/common/utilities/tcptest.c
SRC_C += $(VENDOR)/component/common/api/network/src/wlan_network.c
      
    
#network lwip api
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/api/api_lib.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/api/api_msg.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/api/err.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/api/netbuf.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/api/netdb.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/api/netifapi.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/api/sockets.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/api/tcpip.c
        
      
#network lwip core ipv4
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/ipv4/autoip.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/ipv4/icmp.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/ipv4/igmp.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/ipv4/inet.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/ipv4/inet_chksum.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/ipv4/ip.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/ipv4/ip_addr.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/core/ipv4/ip_frag.c
          
        
#network lwip core       
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
        
      
#network lwip netif
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/src/netif/etharp.c
        
      
#network lwip port
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/port/realtek/freertos/ethernetif.c
SRC_C += $(VENDOR)/component/common/drivers/wlan/realtek/src/osdep/lwip_intf.c
SRC_C += $(VENDOR)/component/common/network/lwip/lwip_v1.4.1/port/realtek/freertos/sys_arch.c
      
#network - wsclient
SRC_C += $(VENDOR)/component/common/network/websocket/wsclient_tls.c  

#network lwip     
SRC_C += $(VENDOR)/component/common/network/dhcp/dhcps.c
SRC_C += $(VENDOR)/component/common/network/sntp/sntp.c
      
    
#network polarssl polarssl
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/aesni.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/blowfish.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/camellia.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/ccm.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/certs.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/cipher.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/cipher_wrap.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/debug.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/ecp_ram.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/entropy.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/entropy_poll.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/error.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/gcm.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/havege.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/md2.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/md4.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/memory_buffer_alloc.c
#SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/net.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/padlock.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/pbkdf2.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/pkcs11.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/pkcs12.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/pkcs5.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/pkparse.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/platform.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/ripemd160.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/ssl_cache.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/ssl_ciphersuites.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/ssl_cli.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/ssl_srv.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/ssl_tls.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/threading.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/timing.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/version.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/version_features.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/x509.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/x509_create.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/x509_crl.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/x509_crt.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/x509_csr.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/x509write_crt.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/x509write_csr.c
SRC_C += $(VENDOR)/component/common/network/ssl/polarssl-1.3.8/library/xtea.c
        
      
#network polarssl ssl_ram_map
SRC_C += $(VENDOR)/component/common/network/ssl/ssl_ram_map/ssl_ram_map.c
        
#os freertos portable
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/portable/MemMang/heap_5.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/portable/GCC/ARM_CM4F/port.c
#SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/portable/IAR/ARM_CM4F/portasm.s
        
      
#os freertos       
SRC_C += $(VENDOR)/component/os/freertos/cmsis_os.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/croutine.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/event_groups.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_service.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/list.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/queue.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/tasks.c
SRC_C += $(VENDOR)/component/os/freertos/freertos_v8.1.2/Source/timers.c
      
    
#os osdep
SRC_C += $(VENDOR)/component/os/os_dep/device_lock.c
SRC_C += $(VENDOR)/component/os/os_dep/osdep_service.c
      
    
  
#peripheral api
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/analogin_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/dma_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/efuse_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/flash_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/gpio_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/gpio_irq_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/i2c_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/i2s_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/nfc_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/pinmap.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/pinmap_common.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/port_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/pwmout_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/rtc_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/serial_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/sleep.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/spi_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/sys_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/timer_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/us_ticker.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/us_ticker_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/wait_api.c
SRC_C += $(VENDOR)/component/common/mbed/targets/hal/rtl8711b/wdt_api.c
      
    
#peripheral rtl8710b
SRC_C += $(VENDOR)/component/soc/realtek/8711b/fwlib/ram_lib/rtl8710b_dsleepcfg.c
SRC_C += $(VENDOR)/component/soc/realtek/8711b/fwlib/ram_lib/rtl8710b_dstandbycfg.c
SRC_C += $(VENDOR)/component/soc/realtek/8711b/fwlib/ram_lib/rtl8710b_intfcfg.c
#SRC_C += $(VENDOR)/component/soc/realtek/8711b/misc/rtl8710b_ota.c
SRC_C += $(VENDOR)/component/soc/realtek/8711b/fwlib/ram_lib/rtl8710b_pinmapcfg.c
SRC_C += $(VENDOR)/component/soc/realtek/8711b/fwlib/ram_lib/rtl8710b_sleepcfg.c
      

     
#utilities example
#SRC_C += $(VENDOR)/component/common/example/bcast/example_bcast.c
#SRC_C += $(VENDOR)/component/common/example/dct/example_dct.c
#SRC_C += $(VENDOR)/component/common/example/eap/example_eap.c
SRC_C += $(VENDOR)/component/common/example/example_entry.c
#SRC_C += $(VENDOR)/component/common/example/get_beacon_frame/example_get_beacon_frame.c
#SRC_C += $(VENDOR)/component/common/example/high_load_memory_use/example_high_load_memory_use.c
#SRC_C += $(VENDOR)/component/common/example/http_client/example_http_client.c
#SRC_C += $(VENDOR)/component/common/example/http_download/example_http_download.c
#SRC_C += $(VENDOR)/component/common/example/mcast/example_mcast.c
#SRC_C += $(VENDOR)/component/common/example/mdns/example_mdns.c
#SRC_C += $(VENDOR)/component/common/example/mqtt/example_mqtt.c
#SRC_C += $(VENDOR)/component/common/example/nonblock_connect/example_nonblock_connect.c
#SRC_C += $(VENDOR)/component/common/example/rarp/example_rarp.c
#SRC_C += $(VENDOR)/component/common/example/sntp_showtime/example_sntp_showtime.c
#SRC_C += $(VENDOR)/component/common/example/socket_select/example_socket_select.c
#SRC_C += $(VENDOR)/component/common/example/ssl_download/example_ssl_download.c
#SRC_C += $(VENDOR)/component/common/example/ssl_server/example_ssl_server.c
#SRC_C += $(VENDOR)/component/common/example/tcp_keepalive/example_tcp_keepalive.c
#SRC_C += $(VENDOR)/component/common/example/uart_atcmd/example_uart_atcmd.c
#SRC_C += $(VENDOR)/component/common/example/wifi_mac_monitor/example_wifi_mac_monitor.c
#SRC_C += $(VENDOR)/component/common/example/wlan_fast_connect/example_wlan_fast_connect.c
#SRC_C += $(VENDOR)/component/common/example/wlan_scenario/example_wlan_scenario.c
#SRC_C += $(VENDOR)/component/common/example/websocket/example_wsclient.c
#SRC_C += $(VENDOR)/component/common/example/xml/example_xml.c
      
    
#utilities 
#SRC_C += $(VENDOR)/component/common/utilities/cJSON.c
#SRC_C += $(VENDOR)/component/common/utilities/http_client.c
#SRC_C += $(VENDOR)/component/common/utilities/uart_socket.c
#SRC_C += $(VENDOR)/component/common/utilities/webserver.c
#SRC_C += $(VENDOR)/component/common/utilities/xml.c
SRC_C += $(VENDOR)/component/soc/realtek/8195a/misc/platform/gcc_wrap.c


SRC_C += main.c


# Initialize target name and target object files
# -------------------------------------------------------------------

all: application

TARGET=application

# Include folder list
# -------------------------------------------------------------------

# Generate obj list
# -------------------------------------------------------------------

SRC_O = $(addprefix $(BUILD)/, $(SRC_C:.c=.o))

OBJ =  $(SRC_O) $(PY_O)

SRC_QSTR += $(SRC_C) $(SRC_C)
SRC_QSTR_AUTO_DEPS +=

################################
# 			CFLAGS 			   #
################################

# Optimize level
CFLAGS = -O2

ifeq ($(DEBUG), 1)
CFLAGS += -g3 -ggdb
else 
CFLAGS += -g2
endif

# source code macro
CFLAGS =
CFLAGS += -DM4 -DCONFIG_PLATFORM_8711B -D$(CHIP)
CFLAGS += -DPOLARSSL_CONFIG_FILE=\"polarssl-config.h\"
CFLAGS += -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g2 -w -O2 -Wno-pointer-sign -fno-common -fmessage-length=0  -ffunction-sections -fdata-sections -fomit-frame-pointer -fno-short-enums -DF_CPU=166000000L -std=gnu99 -fsigned-char
CFLAGS += $(CFLAGS_MOD)
CFLAGS += $(INC)

LFLAGS =
LFLAGS += -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g --specs=nano.specs -nostartfiles -Wl,-Map=$(BUILD)/application.map -Os -Wl,--gc-sections -Wl,--cref -Wl,--entry=Reset_Handler -Wl,--no-enum-size-warning -Wl,--no-wchar-size-warning
LFLAGS += -Wl,-wrap,malloc -Wl,-wrap,free -Wl,-wrap,realloc

LIBFLAGS += -L$(VENDOR)/component/soc/realtek/8711b/misc/bsp/lib/common/GCC/ -l_platform -l_wlan -l_wps -l_dct -l_rtlstd -lm -lc -lnosys -lgcc

RAMALL_BIN = ram_all.bin
IMAGE2_OTA1 = image2_all_ota1.bin
IMAGE2_OTA2 = image2_all_ota2.bin
OTA_ALL = ota_all.bin

application: prerequirement build_info $(SRC_O)  $(PY_O)
	$(ECHO) "Building $(CHIP) "
	$(Q)$(LD) $(LFLAGS) -o $(BUILD)/$(TARGET).axf $(OBJ) $(BUILD)/boot_all.o $(LIBFLAGS) -L$(TOOL)/amebaz -T$(TOOL)/amebaz/MP_RTL8710BN-symbol-v02.ld
	$(Q)$(OBJDUMP) -d $(BUILD)/$(TARGET).axf > $(BUILD)/$(TARGET).asm

.PHONY: manipulate_images
manipulate_images:	
	$(Q)echo ===========================================================
	$(Q)echo Image manipulating
	$(Q)echo ===========================================================
	$(Q)$(NM) $(BUILD)/$(TARGET).axf | sort > $(BUILD)/$(TARGET).nmap

	$(Q)$(OBJCOPY) -j .ram_image2.entry -j .ram_image2.data -j .ram_image2.bss -j .ram_image2.skb.bss -j .ram_heap.data -Obinary $(BUILD)/$(TARGET).axf $(BUILD)/ram_2.r.bin
	$(Q)$(OBJCOPY) -j .xip_image2.text -Obinary $(BUILD)/$(TARGET).axf $(BUILD)/xip_image2.bin
	$(Q)$(OBJCOPY) -j .ram_rdp.text -Obinary $(BUILD)/$(TARGET).axf $(BUILD)/rdp.bin

	$(Q)cp $(VENDOR)/component/soc/realtek/8711b/misc/bsp/image/boot_all.bin $(BUILD)/boot_all.bin
	$(Q)chmod +rw $(BUILD)/boot_all.bin
	$(Q)chmod +rx $(PICK) $(CHKSUM) $(PAD)

	$(Q)$(PICK) 0x`grep __ram_image2_text_start__ $(BUILD)/$(TARGET).nmap | gawk '{print $$1}'` 0x`grep __ram_image2_text_end__ $(BUILD)/$(TARGET).nmap | gawk '{print $$1}'` $(BUILD)/ram_2.r.bin $(BUILD)/ram_2.bin raw
	$(Q)$(PICK) 0x`grep __ram_image2_text_start__ $(BUILD)/$(TARGET).nmap | gawk '{print $$1}'` 0x`grep __ram_image2_text_end__ $(BUILD)/$(TARGET).nmap | gawk '{print $$1}'` $(BUILD)/ram_2.bin $(BUILD)/ram_2.p.bin
	$(Q)$(PICK) 0x`grep __xip_image2_start__ $(BUILD)/$(TARGET).nmap | gawk '{print $$1}'` 0x`grep __xip_image2_start__ $(BUILD)/$(TARGET).nmap | gawk '{print $$1}'` $(BUILD)/xip_image2.bin $(BUILD)/xip_image2.p.bin

	$(Q)cat $(BUILD)/xip_image2.p.bin > $(BUILD)/$(IMAGE2_OTA1)
	$(Q)chmod 777 $(BUILD)/$(IMAGE2_OTA1)
	$(Q)cat $(BUILD)/ram_2.p.bin >> $(BUILD)/$(IMAGE2_OTA1)
	$(Q)$(RM) $(BUILD)/ram_*.bin
	$(Q)$(CHKSUM) $(BUILD)/$(IMAGE2_OTA1) || true
	$(Q)$(SIZE) -d $(BUILD)/$(TARGET).axf

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
	@mv -f .ver inc/amebaz/$@.h


.PHONY: prerequirement
prerequirement:
	$(Q)echo ===========================================================
	$(Q)echo Build $(TARGET)
	$(Q)echo ===========================================================
	$(Q)$(MKDIR) -p $(BUILD)
	$(Q)cp $(VENDOR)/component/soc/realtek/8711b/misc/bsp/image/boot_all.bin $(BUILD)/boot_all.bin
	$(Q)chmod +rw $(BUILD)/boot_all.bin
	$(Q)$(OBJCOPY) -I binary -O elf32-littlearm -B arm $(BUILD)/boot_all.bin $(BUILD)/boot_all.o 

$(SRC_O): $(BUILD)/%.o : %.c
	$(Q)$(MKDIR) -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) $(INC) -c $< -o $@

.PHONY: flashburn
flashburn:
	$(Q)$(CP) -f $(TOOL)/amebaz/target_FPGA.axf $(BUILD)/target_NORMAL.axf
	$(Q)$(CP) -f $(TOOL)/amebaz/SetupGDB_NORMAL.sh $(BUILD)/SetupGDB_NORMAL.sh
	$(Q)$(CP) -f $(TOOL)/amebaz/rtl_gdb_flash_write_openocd.txt $(BUILD)/rtl_gdb_flash_write_openocd.txt
	$(Q)chmod +rw $(BUILD)/target_NORMAL.axf
	$(Q)chmod +rx $(BUILD)/SetupGDB_NORMAL.sh
	$(Q)$(BUILD)/SetupGDB_NORMAL.sh
	$(Q)$(GDB) -x $(BUILD)/rtl_gdb_flash_write_openocd.txt
	
.PHONY: ramdebug
ramdebug:
	$(Q)$(GDB) -x $(TOOL)/amebaz/rtl_gdb_ramdebug_openocd.txt	
