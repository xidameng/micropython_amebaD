print("hello!!!")
from uwireless import WLAN
def test_connect():
    wlan = WLAN(mode=WLAN.STA)
    auth=(WLAN.WPA2_AES_PSK, "22238392abcd")
    wlan.connect(ssid="VIP_WirelessAP", auth=auth, dhcp=True)
    netif = wlan.getnetif()
    print(netif)
