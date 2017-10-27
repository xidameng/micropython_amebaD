from wireless import WLAN
import utime as time
_wlan = WLAN(mode=WLAN.AP)
_wlan.off()
time.sleep_ms(500)
_wlan.on()
def _wlan_assoc_event():
    print("wlan assoc")
def _wlan_disassoc_event():
    print("wlan disassoc")
_wlan.event_handler(WLAN.EVENT_STA_ASSOC, _wlan_assoc_event)
_wlan.event_handler(WLAN.EVENT_STA_DISASSOC, _wlan_disassoc_event)
ap_netif = _wlan.interface(0)
ap_netif.linkup()
ap_netif.default()
ap_netif.ip(('192.168.5.1', '255.255.255.0', '192.168.14.1'))
_wlan.start_ap(ssid='ameba-ap-1', auth=(WLAN.WPA2_AES_PSK, '0987654321'))
