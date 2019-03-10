# Svetozar: an open-source e-vehicles peripherals controller

View this project on [CADLAB.io](https://cadlab.io/project/1290).

_Current project status_: in active development. Some features work, some don't.

## In a nutshell:

<img src="device.png" width="400" height="429" alt="device board">

This ~30$ device allows you to control headlights/turn signals/horn/fan/etc on a typical e-bike/e-scooter.

It consists of a [‘Blue pill’](https://wiki.stm32duino.com/index.php?title=Blue_Pill) board with STM32F103 microcontroller which is widely available
on eBay or [Aliexpress](https://www.aliexpress.com/af/STM32F103C8T6.html?site=glo&origin=n&spm=2114.search0604.0.0.7f4960689ztJHF&filterCat=400103%2C200215223%2C200084026&jump=afs&groupsort=1&SearchText=STM32F103C8T6&SortType=price_asc&initiative_id=SB_20181031001708)
and a custom board with high-voltage circuitry. (You can order this board at [jlcpcb.com](https://jlcpcb.com/) for around 11$/10 pcs delivered.)

An external 12V power source is required. The device is able to switch 6x 12V outputs plus 3x 12..100V outputs
(12..100V nets are optically isolated). One of the 12V outputs is intended to drive a high-current fan and generates a stabilized 0..12V current
for a smooth fan speed control. CAN bus is also supported, so you can write some code to interact with other CAN devices.
There are 11 inputs on the left side, most of them are digital and trigger when connected to ground. ABRK input is analog and accepts voltages up to 5V,
ATEMP input should be used with KTY84/130 temperature sensor (or NTC10k, but you'll have to change input resistors to match the sensor).

## Current functions:

- Stoplight has medium brightness when not triggered
- ABRK & DBRK trigger stoplight to full brightness with a starting blink
- LTURN, RTURN, WARN trigger (left/right/both) turn signals flash with ~1 Hz frequency
- LBEAM triggers LBEAM output
- HBEAM triggers both LBEAM & HBEAM outputs
- HORN triggers HORN output
- AUX triggers AUX output
- BLGHT triggers BLGHT output
- ATEMP can be programmed to smoothly control FAN output
- 12V net triggers current protection at ~25A peak, this disables all 12V outputs 
- BATT+ net triggers current protection at ~12A peak, this disables all 12..100V outputs

## Hardware requirements

* Blue pill board
* Main board with all the components soldered ([BOM](bom.pdf))
* ST-Link/V2 or V2-1 programmer (V1 is supported via native ST software under Windows)

## Firmware requirements

* [Arduino IDE](https://www.arduino.cc/en/Main/Software)
* Arduino SAM boards (Cortex-M3) support installed from Ardiuno IDE's boards manager
* [coddingtonbear's fork](https://github.com/coddingtonbear/Arduino_STM32) of Arduino STM32 ‘hardware’ files with hardware CAN support (`HardwareCAN` git branch)
* [STM32duino bootloader](https://github.com/rogerclarkmelbourne/STM32duino-bootloader/raw/master/binaries/generic_boot20_pc13.bin) for Blue pill boards
* Some STM32 [ST-Link/V2](https://github.com/pavelrevak/pystlink) or [USB-TTL](https://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-programmers/flasher-stm32.html) flashing software

## Flashing a Blue Pill board with the bootloader

### Mac OS X
#### Requirements
##### Software
* python 3.x (brew install python)
* libusb (brew reinstall libusb)
* pyusb (pip3 install pyusb)
* [pystlink](https://github.com/pavelrevak/pystlink)

#### Process
1. Download [STM32duino bootloader](https://github.com/rogerclarkmelbourne/STM32duino-bootloader/raw/master/binaries/generic_boot20_pc13.bin)
2. Connect ST-Link programmer to the board
3. Flash it with
```shell
    pystlink --cpu STM32F103xB flash:erase:verify:generic_boot20_pc13.bin
```
4. Reset or re-connect the board

### Windows
#### To Do

#### Flashing via USB-TTL adapter under Windows
[Please read this russian article](https://habr.com/post/395577/)

## Installing Arduino with STM32duino support
[STM32duino installation instructions](https://github.com/rogerclarkmelbourne/Arduino_STM32/wiki/Installation)

### Mac OS X
1. Install Arduino
2. Install libusb (brew install libusb)
3. Launch Arduino IDE, go to Tools -> Board: ... -> Boards Manager and install the Arduino SAM boards (Cortex-M3) support.
4. Download [coddingtonbear's fork](https://github.com/coddingtonbear/Arduino_STM32) of Arduino STM32 ‘hardware’ files
with hardware CAN support, checkout `HardwareCAN` branch and move and rename it to ```~/Documents/Arduino/hardware/Arduino_STM32```.
5. In Arduino IDE select Tools -> Board: ... -> Generic STM32F103C series.
5. In Arduino IDE select Tools -> CPU speed(MHz): -> 48MHz.

## Credits
Blue pill board library is from [github/sagarHackeD](https://github.com/sagarHackeD/STM32Bluepill_eagle)
VOM1271T.lbr, MAX3051ESA.lbr & MC14504BDR2G.lbr are from [SnapEDA](https://www.snapeda.com/)
