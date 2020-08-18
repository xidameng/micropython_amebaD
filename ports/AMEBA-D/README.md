# MicroPython Ports for Realtek Ameba D

## How to build code ?

```bash
$> make V=1
```

## How to program code ?

For Ameba D, press RESET button while holding down UART Download button, then simply use

```bash
$> make upload
```

To open serial com using picocom 

```bash
$> make com
```

## Default settings

Baudrate: ```115200```
