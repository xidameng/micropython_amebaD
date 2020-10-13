# MicroPython Ameba RTL8722 Documentation
Realtek's RTL8722 is a ARM Cortex-M33 based, dual-band WiFi and BLE 5.0 capable microntroller that is ideal for many IoT applications.

This is a alpha version of the MicroPython port for Ameba RTL8722 platform, details of the platform can be found here  https://www.amebaiot.com/en/amebad/


## 1. How to build firmware?
Currently, this SDK only support building on Cygwin or Linux.

Before preceed, please make sure that you have already installed GNU ```make``` and the latest ```Python3```.

Open Cygwin terminal/Ubuntu terminal and navigate to "\micropython_amebaD\MicroPython_RTL8722\ports\rtl8722" and then type,

```bash
$ make
```

### 1.1 FAQ
During the building process, some user may encounter error that suspend the process, this is due to the system enevironment setup and can be fixed as follows,

#### Error related to python
By default, MicroPython use ```Python3``` to run some building scripts for the MicroPython kernal, if you encounter error related to python, it may be because the path of the Python3 executable is not added to system environment variable.

However, if environment variable is already added but still not working, you may try,
1) Re-start your PC
2) type "python" on your terminal, if the python shown is python3, then please add 
```bash
PYTHON = python
``` 
to the second line of the Makefile in our "port/rtl8722" folder

#### Error related to MPY-CROSS
If building process stop when ```mpy-cross``` shown as error, there is a step to be done as follows,
1) navigate to "MicroPython_RTL8722/mpy-cross" folder
2) Open your Cygwin/Linux terminal and just type 
```bash
$ make
```
Wait for make finish building the MicroPython Cross Compiler, then this should fix the error

## 2. How to upload?
There are 2 methods to upload Ameba D MicroPython image to your Ameba.

### 2.1 Release folder
In the release folder, there is a ```Double-Click-Me-to-Upload.cmd``` file. 

First we right click on it and select 'Edit', and a notepad will open, now check your Ameba's serial COM port on your PC and update the correct one to the third last line of the file, then save the file and close it.

Now press RESET button while holding down UART Download button to enter ```Download Mode```, and you can double click ```Double-Click-Me-to-Upload.cmd``` now and the uploading will start shortly.

### 2.2 port/rtl8722 folder
1st, check your ameba Serial/COM port, make sure Ameba D's port name is correctly updated in the ```UPLOAD_PATH``` variable in the Makefile;

2nd, press RESET button while holding down UART Download button to enter ```Download Mode```,
Then, type following command,

```bash
$ make upload
```


## 3. How to use MicroPython RTL8722 Port?

### 3.1 Brief introduction to MicroPython RTL8722 port
MicroPython distinguishes itself from other compilation-based platforms (Arduino etc.) with its powerful method of real-time interaction with Microcontroller through a built-in feature --  ```REPL```. 

REPL stands for Read-Evaluation-Print-Loop, it's an interactive prompt that you can use to access and control your microcontroller.

REPL has been equipped with other powerful features like tab completion, line editing, auto-indentation, input history and more. It basically functions like the classic Python IDLE but running on microcontroller.

To use REPL, simply open any serial terminal software (most common ones are teraterm, putty etc.) on your PC and connect to your microcontroller's serial port, then set baudrate to ```115200``` before reset the board, then you will see ```>>>``` MicroPython prompt appear on the console. Now you may type in any Python script on REPL as long as it's support by MicroPython and your microcontroller's MicroPython port.

### 3.2 REPL Hotkeys
1. Ctrl + d : Soft reboot
	MicroPython will perform software reboot, this is useful when your microcontroller is behaving abnormally. This will also run scripts in 'boot.py' once again.

2. Ctrl + e : Paste mode
	Paste mode allow you to perform pasting a large trunk of code into REPL at once without executing code line by line. This is useful when you have found a MicroPython library and wish to test it out immediately by copy and paste

3. Ctrl + b : Normal mode
	This hotkey will set REPL back to normal mode. This is useful if you are stuck in certain mode and can not get out.

4. Ctrl + c : Quick cancel
	This hotkey help you to cancel any input or interrupt currently running code


## 4. Peripheral Control -- umachine module
MicroPython Ameba D port supports rich peripheral feature through the use of ```umachine``` module

### GPIO
To control GPIO, import ```Pin``` module through ```umachine```. Here pin PB_18 is used as an example to output logic level 0 and 1 and blink 3 times 

```bash
from umachine import Pin
a = Pin("PB_18", Pin.OUT)
a.value(1)
time.sleep_ms(500)
a.value(0)
time.sleep_ms(500)
a.on()
time.sleep_ms(500)
a.off()
time.sleep_ms(500)
a.toggle()
time.sleep_ms(500)
a.toggle()
```


### PWM
To use PWM (Pulse Width Modulation), import ```PWM``` module through ```umachine```. Here pin PA_26 is used as an example to make an LED to slowly brighten up

```bash
from umachine import Pin, PWM
import time
p = PWM(pin = "PA_26")
# 0 duty cycle thus output 0
p.write(0.0)
# 10% duty cycle
p.write(0.1)
# 50% duty cycle
p.write(0.5)
# 100% duty cycle
p.write(1.0)
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

There are 4 sets of 32KHz General Timers available to user, Timer 0/1/2/3

```bash
from umachine import Timer
t = Timer(0)  # Use Timer 0/1/2/3 only
t.start(2000000, t.PERIODICAL)  # Set GTimer fired periodically at duration of 2 seconds, printing text on the terminal
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
uart.init()
uart.write('hello')
uart.read(5) # read up to 5 bytes
```


### I2C
Use the ```I2C``` (Inter-Integrated Circuit) module through ```umachine``` module

Note: I2C only works in ```master``` mode.

```bash
from umachine import Pin, I2C
i2c = I2C(scl = "PA_25", sda = "PA_26", freq=100000) # configure I2C with pins and freq. of 100KHz
i2c.scan()
i2c.writeto(8, 'hello') # send 5 bytes to slave with address 8
i2c.readfrom(8, 6) # receive 5 bytes from slave
```


### SPI 
Use the ```SPI``` (Serial Peripheral Interface) module through ```umachine``` module, currently only support ```Master``` mode, default SPI baud rate is ```2MHz```.

```bash
from umachine import SPI
spi = SPI(0)		# Only support 2 sets of SPI -- 0 and 1 
spi 				# type instance name to check for details of the SPI set 
spi.write(123)		# Write number 123 
spi.read()
```

## Netowork
RTL8722 MicroPython port support WiFi connection through ```WLAN``` module.
### WiFi

Connect to WiFi with WPA2 password
```bash
from wireless import WLAN
wifi = WLAN(mode = WLAN.STA)
wifi.connect(ssid = "MPtestSSID", pswd = "micropythonameba")
```
Scan network
```bash
from wireless import WLAN
wifi = WLAN(mode = WLAN.STA)
wifi.scan()
```