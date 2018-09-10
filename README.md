View this project on [CADLAB.io](https://cadlab.io/project/1290). 

# Svetozar: an open-source e-vehicles peripherals controller 
This device allows you to control headlights/horn/fan/etc on a typical e-bike/e-scooter. Based on a STM32F103 [Blue pill board](https://wiki.stm32duino.com/index.php?title=Blue_Pill).

Supply power: 12V. Switches 12-150V devices. Supports CAN bus and [LEVCAN](https://github.com/VasiliSker/LEVCAN/) protocol.

## Flashing Blue Pill board

### Mac OS X
#### Requirements
##### Software
* python 3.x (brew install python)
* libusb (brew reinstall libusb)
* pyusb (pip3 install pyusb)
* [pystlink](https://github.com/pavelrevak/pystlink)
##### Hardware
* ST-Link/V2 or V2-1 programmer (pystlink does not support V1, please use native ST software under Windows)

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
4. Download [this file](http://arduino.cc/en/Main/Software), unpack it and move and rename it to ```~/Documents/Arduino/hardware/Arduino_STM32```.
5. In Arduino IDE select Tools -> Board: ... -> Generic STM32F103C series.
