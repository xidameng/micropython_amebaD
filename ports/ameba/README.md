# MicroPython Ports for Realtek Amena series (RTL8195A, RTL8711BN)

## How to init project ?

```bash
$> git submodule init
$> git submodule update
```

## How to build code ?

```bash
$> make V=1
```

## How to program code ?

For Ameba D, simple use

```bash
$> make upload
```

Open a terminal , use your jlink jtag programmer

```bash
$> openocd -f interface/jlink.cfg -f tool/rtl8195a/ameba1.cfg
```

Then, open another terminal

```bash
$> make flashburn
```

## Default settings

Baudrate: ```38400```
