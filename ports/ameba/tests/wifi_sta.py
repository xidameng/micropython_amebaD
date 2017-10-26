from wireless import WLAN
import utime as time

def _wlan_default_connect_callback():
    print("wlan connected")

def _wlan_default_disconnect_callback():
    print("wlan disconnect")

_wlan = WLAN(mode=WLAN.STA)
_wlan.off()
_wlan.on()
_wlan.event_handler(WLAN.EVENT_CONNECT, _wlan_default_connect_callback)
_wlan.event_handler(WLAN.EVENT_DISCONNECT, _wlan_default_disconnect_callback)
if _wlan.wifi_is_running(0) is not True:
    print("WLAN driver is not running")
try:
    _wlan.connect(ssid="YOUR-SSID", auth=(WLAN.WPA2_AES_PSK, "YOUR-PASSWORD"))
except OSError as e:
    print("connect to YOUR-SSID failed. {}".format(e))

timeout = 2000
netif = _wlan.interface(0)
while timeout:
    time.sleep_ms(1)
    if _wlan.is_connect_to_ap() is True:
        netif.dhcp_request(10)
        break
    timeout-=1
print(netif.ip())
