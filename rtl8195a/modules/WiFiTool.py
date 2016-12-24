from uwireless import WLAN

class SimpleConnect:
    def __init__(self, ssid, password):
        self.ssid = ssid
        self.password = password
        self.wlan = WLAN(mode=WLAN.STA)
        self.wlan.on()

    def is_wifi_running(self):
        return self.wlan.wifi_is_running()
