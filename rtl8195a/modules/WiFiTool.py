from uwireless import WLAN
from network import NETIF
import utime

SEC_WPA2 = WLAN.WPA2_AES_PSK
SEC_OPEN = WLAN.OPEN

class SimpleConnect:
    def __init__(self, ssid, password, security=SEC_WPA2, disconnect=None, connect=None):
        self.ssid = ssid
        self.password = password
        self.security = security
        self.wlan = WLAN(mode=WLAN.STA)
        self.wlan.off()
        self.wlan.on()
        self.is_connected = False
        if connect is None:
            connect = self.wlan_connect_callback
        if disconnect is None:
            disconnect = self.wlan_disconnect_callback
        self.wlan.event_handler(WLAN.EVENT_CONNECT, connect)
        self.wlan.event_handler(WLAN.EVENT_DISCONNECT, disconnect)

    @property
    def wifi_is_running(self):
        return self.wlan.wifi_is_running(0)

    @property
    def is_connect_to_ap(self):
        return self.wlan.is_connect_to_ap()

    def wlan_connect_callback():
        print("wlan connected!")

    def wlan_disconnect_callback():
        print("wlan disconnect")

    def connect(self, timeout=2000):
        if self.wifi_is_running != True:
            raise OSError("wifi is not running")
        try:
            self.wlan.connect(ssid=self.ssid, auth=(self.security, self.password))
        except OSError as e:
            raise OSError("connect to %s failed" % self.ssid)

        while timeout:
            utime.sleep_ms(1)
            if self.is_connect_to_ap is True:
                netif = NETIF(0)
                netif.dhcp_request(10)
                print(netif)
                return
            timeout -= 1

        raise OSError("STA has not connect to AP")

def wifi_connect():
    ssid = "VIP_WirelessAP"
    #ssid = "LIN1"
    password = "22238392abcd"
    #password = "12345678"

    def wlan_connected():
        print("wlan connected")

    def wlan_disconnected():
        print("wlan disconnected")

    wlan = SimpleConnect(ssid=ssid, password=password, connect=wlan_connected, disconnect=wlan_disconnected);
    wlan.connect()
