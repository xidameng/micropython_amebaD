from wireless import WLAN
import utime as time
_wlan = WLAN(mode=WLAN.AP)
_wlan.off()
time.sleep_ms(500)
_wlan.on()
ap_netif = _wlan.interface(0)
ap_netif.linkup()
ap_netif.ip(('192.168.14.1', '255.255.255.0', '192.168.14.1'))
_wlan.start_ap(ssid='ameba-ap-1', auth=(WLAN.WPA2_AES_PSK, '0987654321'))
