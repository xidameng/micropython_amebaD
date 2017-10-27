from wireless import WLAN
import utime as time
def _wlan_default_connect_event():
    print("wlan connected")

def _wlan_default_disconnect_event():
    print("wlan disconnect")

def _wlan_assoc_event():
    print("wlan assoc")

def _wlan_disassoc_event():
    print("wlan disassoc")

_wlan = WLAN(mode=WLAN.STA_AP)
_wlan.off()
time.sleep_ms(500)
_wlan.on()

if _wlan.wifi_is_running(0) is not True:
    print("WLAN driver is not running")

_wlan.event_handler(WLAN.EVENT_STA_ASSOC, _wlan_assoc_event)
_wlan.event_handler(WLAN.EVENT_STA_DISASSOC, _wlan_disassoc_event)
_wlan.event_handler(WLAN.EVENT_CONNECT, _wlan_default_connect_event)
_wlan.event_handler(WLAN.EVENT_DISCONNECT, _wlan_default_disconnect_event)

ap_netif = _wlan.interface(1)
sta_netif = _wlan.interface(0)
ap_netif.linkup()
sta_netif.linkup()
ap_netif.default()
ap_netif.ip(('192.168.5.1', '255.255.255.0', '192.168.14.1'))
_wlan.start_ap(ssid='ameba-ap-1', auth=(WLAN.WPA2_AES_PSK, '0987654321'))

try:
    _wlan.connect(ssid="YOUR-SSID", auth=(WLAN.WPA2_AES_PSK, "YOUR-PASSWORD"))
except OSError as e:
    print("connect to YOUR-SSID failed. {}".format(e))
