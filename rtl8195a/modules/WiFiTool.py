from uwireless import WLAN
import utime, ujson

def _wlan_default_connect_callback():
    print("wlan connected")

def _wlan_default_disconnect_callback():
    print("wlan disconnected")

class WlanConfig:
    def __init__(self, json=None, config_path="/flash/.wlanconfig", read_from_file=False):
        self.config_path = config_path
        self.mode = WLAN.STA
        self.sta = dict(ssid = None, security=WLAN.OPEN, password=None)
        self.ap = dict(ssid="mpiot-ap", security=WLAN.WPA2_AES_PSK, password="1234567890")
        if json is not None:
            config = ujson.loads(json)
            try:
                self.mode = config['mode']
            except KeyError as e:
                raise KeyError("Can not find wlan mode in json config")
            self.sta = config['sta']
            self.ap = config['ap']

    @property
    def json(self):
        return ujson.dumps(self.__dict__)

    @property
    def dict(self):
        return self.__dict__

    def save(self):
        with open(self.config_path, "w") as f:
            f.seek(0)
            f.write(self.json)

    def load(self):
        try:
            with open(self.config_path, "r") as f:
                f.seek(0)
                config_json = f.read()
                self.__init__(json=config_json, read_from_file=False)
        except OSError as e:
            raise OSError("Load config: %s failed" % self.config_path)


_config = WlanConfig(config_path='/flash/.wlanconfig')
_wlan = WLAN(mode=_config.mode)
_wlan.on()

def simple_connect(timeout=2000, dhcp=True, load_config=True):
    global _config
    global _wlan
    if load_config:
        try:
            _config.load()
        except OSError as e:
            raise OSError("error loading config file")
    _wlan.event_handler(WLAN.EVENT_CONNECT, _wlan_default_connect_callback)
    _wlan.event_handler(WLAN.EVENT_DISCONNECT, _wlan_default_disconnect_callback)
    sta_config = _config.sta
    if _wlan.wifi_is_running(0) != True:
        raise OSError("WiFi is not running")
    try:
        _wlan.connect(ssid=sta_config['ssid'], auth=(sta_config['security'], sta_config['password']))
    except OSError as e:
        raise OSError("connect to %s failed" % sta_config['ssid'])

    if dhcp is True:
        while timeout:
            utime.sleep_ms(1)
            if _wlan.is_connect_to_ap() is True:
                netif = _wlan.interface(0)
                netif.dhcp_request(10)
                return
            timeout -= 1
        raise OSError("STA did not connect to AP")

def bootstrap():
    mode_choice = {"sta": _wlan.STA, "ap": _wlan.AP, "sta_ap": _wlan.STA_AP, "promisc": _wlan.PROMISC}
    sta_security = {"open": _wlan.OPEN, "wpa_psk": _wlan.WEP_PSK, "wep_shared": _wlan.WEP_SHARED, 
            "wpa_tkip_psk": _wlan.WPA_TKIP_PSK, "wpa2_aes_psk": _wlan.WPA2_AES_PSK, "wpa2_mixed_psk": _wlan.WPA2_MIXED_PSK,
            "wpa_wpa2_mixed": _wlan.WPA_WPA2_MIXED, "wpa_open": _wlan.WPS_OPEN, "wpa_secure": _wlan.WPS_SECURE}
    prompt = "===>"
    print("== This is WiFiTool bootstraping ==")
    print("Now please follow the guide to generate WiFi config file")
    print("Reseting WLAN ...")

    _wlan.off()

    print("Please choose WLAN mode: ")
    for key, item in enumerate(mode_choice):
        print("%s" % key)

    mode = input(prompt)

    try:
        choosed_mode = mode_choice[mode.lower()]
    except:
        raise ValueError("Invalid mode selection")

    config = dict()

    if choosed_mode == _wlan.STA or choosed_mode == _wlan.STA_AP:
        print("Please the AP's SSID you want to connect to")
        ssid = input(prompt)
        print("Please select AP's security type you want to connect to")
        print("Avaliable choices:")
        for key, item in enumerate(sta_security):
            print(key)
        security = input(prompt)
        print("Please the AP's password you want to connect to")
        password = input(prompt)
    elif choosed_mode == _wlan.AP or choosed_mode == _wlan.STA_AP:
        pass
    elif choosed_mode == _wlan.PROMISC:
        pass
    else:
        pass
