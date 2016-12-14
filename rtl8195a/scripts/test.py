print("hello!!!")
from uwireless import WLAN
import utime
import umachine, uos
def test_connect():
    wlan = WLAN(mode=WLAN.STA)
    auth=(WLAN.WPA2_AES_PSK, "22238392abcd")
    wlan.connect(ssid="VIP_WirelessAP", auth=auth, dhcp=True)
    netif = wlan.getnetif()
    print(netif)


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
