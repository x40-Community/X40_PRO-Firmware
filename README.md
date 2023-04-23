# WEEDO X40 PRO Community Firmware
![image](http://www.x40-community.org/images/x40/Weedo_X40_PRO.png)

## Information
The use of the community firmware is at your own risk. There is no liability whatsoever for any damage! 

[Support only for X40-Community member!](http://www.x40-community.org/index.php/members-area)

The X40 PRO firmware has been optimized to work with Ultimaker Cura. If you use a different slicer, the printer may not work properly! [Please use Ultimaker Cura with X40 PRO Cura configuration to avoid any issues.](https://github.com/x40-Community/Cura_for_Weedo_X40_PRO)

The Cura printing profiles are matched to the [X40 PRO Part Community cooling system.](https://github.com/x40-Community/X40_PRO-Community_Coolingsystem) If you use the original Weedo X40 Part cooling system, the settings may not be correct or the pressure may not work properly!

Please note that the High Temperature variant only works with the [High Temperature Upgrade.](http://www.x40-community.org/index.php/firmware/x40-pro-high-temperature) Without the upgrade you will destroy the printer with highly toxic fumes!

## Summary
This is the repository that contains the community version firmware for the WEEDO X40 PRO 3D Printer. The X40 PRO firmware is a further development of the community firmware provided by Weedo. Besides bug fixes there are also the following features:
- 25 Points bed leveling
- XY-Offset SuperFine tuning with a settings accuray of 0.01
- Z-Offset Fine tinung with a settings accuray of 0.02
- Printer no longer stops over the print object when changing tools Switchable wiping function (remove material from pre-extrusion)
- Switchable pre-extrusion function
- Optimized manual bed leveling and Z-offset with 0.04mm steps
- Babysteps with 0.02mm steps
- Extended material selection with temperature indication
- Reduced filament feed speed
- Extended parameter display and backup firmware setup function
- Optimized firmware parameters against heating failed error
- Bug fix that eliminate crashs
- Enable G3 arc command
- Add M0 pause command
- Add M922 Turn automatic filament extrusion on/off command
- Add M923 Turn multiple nozzle wipes on/off command
- BMG firmware variant enables use of mirrored extruders (option)

More information see http://www.x40-community.org/index.php/firmware/x40-pro-firmware

## Usable Hardware Version 
- X40 V1: The first mass production version. The production time is from November 2020 to June 2021.
- X40 V2: The second production version. The production batch start from July 2021.
The length in the Y-axis direction has been shortened. The Y-axis coordinate has been changed from (-2,310) to (-11,303).

## Compile requirements

- Download and install [VSCode](https://code.visualstudio.com/)
- Search and install PlatformIO IDE from store marketplace
- Search and install ST STM32 embedded platform from PIO Home
- Copy framework-arduinoststm32-maple@99.99.99 from /buildroot/lib to PlatformIO library location (user)/.platformio/packages/
- Install the USB driver from /buildroot/driver/CH341SER.ZIP

## Upload firmware

X40 uses a customized bootloader, which requires a customized download program for firmware update.  

The Windows version of the download program WEEDOIAP.exe is located in the /buildroot/upload/ directory. The macos version is still under development.

Lanuch the WEEDOIAP.exe, open the firmware.bin file from build directory, choice the com port, then click the update.

![image](http://www.weedo.ltd/wp-content/uploads/2021/04/weedoiap.png)

X40 will restart to enter the firmware update mode. Wait for about 1 minute, the printer will automatically restart after the firmware update is completed.

![image](http://www.weedo.ltd/wp-content/uploads/2021/04/iap.jpg)



## The difference between the X40 PRO community version firmware and the Weedo Stock firmware

The stock firmware has the function of upgrading the firmware via TF card. Use the IAP function in the control menu to read the flash.wfm file on the TF card to upgrade the firmware.

The X40 PRO community version firmware does not support the wfm format, so it does not support firmware upgrade via TF card. Please use the flashing software for an up or down grade.


## Return to the official vesrion firmware

Use WEEDOIAP.exe to flash X40_Vxxx_Community_to_Official.bin in the /Release directory from the Weedo source code.
