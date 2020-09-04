# MicroPython Ameba D Documentation

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


## Peripheral Control
MicroPython Ameba D port supports rich peripheral feature through the use of ```umachine``` module

### GPIO
To control GPIO, import ```Pin``` module through ```umachine```. Here pin PB_18 is used as an example to output logic level 0 and 1

```bash
>>> from umachine import Pin
>>> a = Pin("PB_18", Pin.OUT)
>>> a.value(1)
>>> a.value(0)
```

### Delay and Timing
Use the ```time``` module

```bash
>>> import time
>>> time.sleep(1)           # sleep for 1 second
>>> time.sleep_ms(500)      # sleep for 500 milliseconds
>>> time.sleep_us(10)       # sleep for 10 microseconds
>>> start = time.ticks_ms() # get millisecond counter
```
### I2C
```bash
>>> from umachine import Pin, I2C
>>> i2c = I2C(scl=Pin("PA_25",Pin.PULL_UP), sda=Pin("PA_26",Pin.PULL_UP), freq=100000)
```