print("hello!!!")
from uwireless import WLAN
import utime
import umachine, uos
import WiFiTool

def sdio_test(count, delay):
    global sdio_vfs
    global sdio

    sdio = umachine.SDIO_HOST()
    sdio_vfs = uos.VfsFat(sdio, "/sdio")
    for i in range(count):
        f = sdio_vfs.open("/sdio/test_%d.txt" % i, "w")
        f.write(utime.ctime())
        utime.sleep_ms(delay)
        f.close()
        utime.sleep_ms(delay)
        print("i = %d" % i)

def wifi_connect():
    ssid = "VIP_WirelessAP"
    password = "22238392abcd"

    def wlan_connected():
        print("wlan connected")

    def wlan_disconnected():
        print("wlan disconnected")

    wlan = WiFiTool.SimpleConnect(ssid=ssid, password=password, connect=wlan_connected, disconnect=wlan_disconnected);
    wlan.connect()
