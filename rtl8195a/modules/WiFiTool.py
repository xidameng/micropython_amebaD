from uwireless import WLAN
import utime, ujson

SEC_WPA2 = WLAN.WPA2_AES_PSK
SEC_OPEN = WLAN.OPEN

def wlan_default_connect_callback():
    print("wlan connected")

def wlan_default_disconnect_callback():
    print("wlan disconnected")

class SimpleConnect:

    CONFIG_SSID="ssid"
    CONFIG_MODE="mode"
    CONFIG_SECURITY="security"
    CONFIG_PASS="password"

    def __init__(self, config_file=".wlanconfig", disconnect=None, connect=None):
        try:
            f = open(config_file, "r")
        except:
            raise ValueError("Open .wlanconfig failed")

        f.seek(0)
        config_json = f.read()
        config = ujson.loads(str(config_json))
        f.close()

        try:
            self.ssid = config[self.CONFIG_SSID]
            self.wlan_mode = config[self.CONFIG_MODE]
            self.security = config[self.CONFIG_SECURITY]
            self.password = config[self.CONFIG_PASS]
        except KeyError as e:
            raise KeyError(e)

        self.wlan = WLAN(mode=self.wlan_mode)
        self.wlan.off()
        self.wlan.on()
        self.is_connected = False
        if connect is None:
            connect = wlan_default_connect_callback
        if disconnect is None:
            disconnect = wlan_default_disconnect_callback
        self.wlan.event_handler(WLAN.EVENT_CONNECT, connect)
        self.wlan.event_handler(WLAN.EVENT_DISCONNECT, disconnect)

    @property
    def wifi_is_running(self):
        return self.wlan.wifi_is_running(0)

    @property
    def is_connect_to_ap(self):
        return self.wlan.is_connect_to_ap()

    @property
    def netif_0(self):
        return self.wlan.interface(0)

    @property
    def netif_1(self):
        return self.wlan.interface(1)

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
                netif = self.wlan.interface(0)
                netif.dhcp_request(10)
                return
            timeout -= 1

        raise OSError("STA has not connect to AP")
