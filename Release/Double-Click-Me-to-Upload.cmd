@echo ===========================
@echo !                         !
@echo !       IMPORTANT !       !
@echo !                         !
@echo ===========================
@echo Check your ameba's serial port and update this file accordingly before setting your ameba to UART Download mode and upload the image
:: mode > device.txt
:: find "COM" device.txt
pause
amebad_image_tool.exe COM8 
:: Change COM8 to your ameba's COM port
pause