# MicroPython Ameba RTL8722 Documentation
Realtek's RTL8722 is a ARM Cortex-M33 based, dual-band WiFi and BLE 5.0 capable microntroller that is ideal for many IoT applications.

This is a alpha version of the MicroPython port for Ameba RTL8722 platform, details of the platform can be found here  https://www.amebaiot.com/en/amebad/


## How to build code?
Currently, this SDK only support building on Cygwin or Linux.

Before preceed, please make sure that you have already installed GNU ```make```

Open Cygwin terminal/Ubuntu terminal and type,

```bash
$ make
```


## How to upload?

1st, check your ameba Serial/COM port, make sure Ameba D's port name is correctly updated in the ```UPLOAD_PATH``` variable in the Makefile;

2nd, press RESET button while holding down UART Download button to enter ```Download Mode```,
Then, type following command,

```bash
$ make upload
```


## How to use MicroPython?
Open any serial terminal software on your PC, most common ones are teraterm, putty etc. and set baudrate to ```115200``` before reset the board, then you will see ```>>>``` MicroPython prompt appear on the console.


## Peripheral Control -- umachine module
MicroPython Ameba D port supports rich peripheral feature through the use of ```umachine``` module

### GPIO
To control GPIO, import ```Pin``` module through ```umachine```. Here pin PB_18 is used as an example to output logic level 0 and 1

```bash
from umachine import Pin
a = Pin("PB_18", Pin.OUT)
a.value(1)
a.value(0)
```


### PWM
To use PWM (Pulse Width Modulation), import ```PWM``` module through ```umachine```. Here pin PA_26 is used as an example to make an LED to slowly brighten up

```bash
from umachine import Pin, PWM
import time as t
p = PWM(pin = "PA_26")
for i in range(1000):
p.pulsewidth(i)
t.sleep_ms(2)
```


### Delay and Timing
Use the ```time``` module

```bash
import time
time.sleep(1)           # sleep for 1 second
time.sleep_ms(500)      # sleep for 500 milliseconds
time.sleep_us(10)       # sleep for 10 microseconds
start = time.ticks_ms() # get millisecond counter
```


### Timer
Use the ```Timer``` module through ```umachine``` module

There are 4 sets of 32K GTimer available to user, Timer 0/1/2/3

```bash
from umachine import Timer
t = Timer(0)  # Use Timer 0/1/2/3 only
t.init()      # Initialize Gtimer
t.start(2000000, lambda :print("timer fired"), t.PERIODICAL)  # Set GTimer at duration of 2 seconds, with a lambda callback function and fired periodically
```


### RTC
Use the ```RTC``` (Real Time Clock) module through ```umachine``` module

```bash
from umachine import RTC
rtc = RTC()
rtc.datetime((2020, 12, 31, 4, 23, 58, 59, 0)) # set a specific date and time (year, month, day, weekday(0 for Monday), hour, minute, second, total seconds)
rtc.datetime() # get date and time
```


### UART
Use the ```UART``` module through ```umachine``` module

```bash
from umachine import UART
uart = UART(tx="PA_21", rx= "PA_22")
uart.write('hello')
uart.read(5) # read up to 5 bytes
```


### I2C
Use the ```I2C``` (Inter-Integrated Circuit) module through ```umachine``` module
```bash
from umachine import Pin, I2C
i2c = I2C(scl = "PA_25", sda = "PA_26", freq=100000) # configure I2C with pins and freq. of 100KHz
i2c.scan()
i2c.writeto(0x42, 'hello') # send 5 bytes to slave with address 0x42
i2c.readfrom(0x42, 5) # receive 5 bytes from slave
```


### SPI (WIP)
Use the ```SPI``` (Serial Peripheral Interface) module through ```umachine``` module
```bash
from umachine import SPI
spi = SPI(0, SPI.MASTER, baudrate=20000000, polarity=0, phase=0)
spi.write('hello')
spi.read(5)                       # receive 5 bytes on the bus
rbuf = bytearray(5)
spi.write_readinto('hello', rbuf) # send and receive 5 bytes
```
