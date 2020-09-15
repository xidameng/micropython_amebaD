# MicroPython Ameba RTL8722 Documentation

## How to build code?
Open Cygwin terminal/Ubuntu terminal and type,

```bash
$ make
```

## How to upload?

First, check your COM port, make sure Ameba D's port is in line with the ```UPLOAD_PATH``` variable in Makefile, 
Second, press RESET button while holding down UART Download button to enter ```Download Mode```,
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

### UART
Use the ```UART``` module through ```umachine``` module

```bash
from umachine import UART, Pin
u = UART()



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
Use the ```RTC``` module through ```umachine``` module

```bash
from umachine import RTC
rtc = RTC()
rtc.datetime((2020, 12, 31, 4, 23, 58, 59, 0)) # set a specific date and time (year, month, day, weekday(0 for Monday), hour, minute, second, total seconds)
rtc.datetime() # get date and time
```

### I2C bus
Use the ```I2C``` module through ```umachine``` module
```bash
from umachine import Pin, I2C
i2c = I2C(scl = "PA_25", sda = "PA_26", freq=100000)
i2c.scan()

```