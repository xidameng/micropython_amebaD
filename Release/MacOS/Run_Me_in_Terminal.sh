#!/bin/bash
chmod 777 amebad_image_tool
echo
echo ==========
echo IMPORTANT            
echo ==========
echo
echo Note: There might be popup window asking your permission to execute thrid party executable file, just click yes/ok to give permission 
pause
echo -Firstly, set your ameba to UART Download mode by holding down UART Download button and press RESET button
echo
echo -Secondly, check your ameba serial port and type it down below
echo For example: /dev/tty.usbserialxxxxxx
read COM
./amebad_image_tool $COM
