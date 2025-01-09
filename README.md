![Cover](https://github.com/KonradWohlfahrt/GlowtieV2/blob/main/images/Cover.jpg)

# Glowtie V2
**Version 2 of my Glowtie adaptation: More glow for less (bow)tie!**
In this repo you will find the new project files for version 2 of the Glowtie!
It was originally designed by [Stephen Hawes](https://github.com/sphawes/glowtie).
Version 1 has served me well, but I still wanted to impove on the features and its look!
The old ESP8266 chip and Micro USB socket were exchanged with USB-C and the ESP32 C3.
Apart from two buttons and an I2S microphone, four more WS2812b leds were added to a slightly smaller pcb.
Finally, this project was made possible by [PCBWay](https://www.pcbway.com/)!
The awesome looking pcb for this project was fabricated by them.
Like last time I was satisfied with the quality and the black finish.
Creating circuit boards can be a tough task, but with such services it's made fast and simple!
If you have a project that involves pcb prototype, PCBWay offers services, such as pcb production, assembly as well as 3D printing and CNC machining for a small price!
Check out my full [guide]() at Instructables if you want to build one yourself too.

***
# Materials:
| Component | Amount | Silkscreen label |
|:----------|:------:|-----------------:|
| custom pcb | 1 | - |
| 3D printed case | 1 | - |
| 500 mAh LiPo battery (max size: 25mm x 35mm x 7mm) | 1 | - |
| M3x6 screw with nut | 2 | - |
| 100uF 1206 | 1 | C5 |
| 10uF 0805 | 3 | C2, C7, C8 |
| 1uF 0805 | 2 | C4, C6 |
| 100nF 0805 | 20 | C1, C3, C9-C27 |
| LED red 0805 | 1 | LED1 |
| LED green 0805 | 1 | LED2 |
| WS2812b 5050 | 17 | LED3-LED19 |
| 100r 0805 | 1 | R4 |
| 330r 0805 | 1 | R10 |
| 1k 0805 | 3 | R2, R3, R5 |
| 3k 0805 (charge programming resistor) | 1 | R1 |
| 10k 0805 | 5 | R6-R9, R12 |
| 33k 0805 | 1 | R11 |
| TP4056 | 1 | U4 |
| DW01A | 1 | U5 |
| FS8205A | 1 | Q1 |
| MCP1826S-3302 | 1 | U3 |
| ESP32-C3-WROOM-02 | 1 | U1 |
| SMD button 3 * 6 * 2.5 | 2 | SW2, SW3 |
| USB Type-C 4 pin | 1 | USB1 |
| SK12D07VG6 | 1 | SW1 |
| INMP441 I2S mic module | 1 | U2 |

***
# Schematic:
![Schematic](https://github.com/KonradWohlfahrt/GlowtieV2/blob/main/images/Schematic_Glowtie-V2.png)
_Schematic for Version 2_
![SolderingLables](https://github.com/KonradWohlfahrt/GlowtieV2/blob/main/images/GlowtieV2_Labels.jpg)
_Lables for soldering components_

**Note:**
During the development I have discovered an issue regarding the WiFi-Version:
I connected ADC Channel 2 (GPIO5) with the I2S microphone. During WiFi operation it won't be available!
Thus, you cannot use the music reactive effects while keeping WiFi enabled. 

***
# Programming
Programmed with the Arduino IDE and ESP32 board manager. 
Install the following libraries:

WiFi Version
- [WS2812FX](https://github.com/kitesurfer1404/WS2812FX)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)

BLE/Mic Version
- [WS2812FX](https://github.com/kitesurfer1404/WS2812FX)
- [ArduinoFFT](https://github.com/kosme/arduinoFFT)

**Uploading:**
Board: `ESP32C3 Dev Module`
Settings: default
Connections for FTDI programmer (3.3V logic level):
- Glowtie -> FTDI
- TX -> RX
- RX -> TX
- VCC -> VCC
- GND -> GND
Press and hold the upper button (Flash) when powering up the programmer until the sketch started uploading.
![Programming](https://github.com/KonradWohlfahrt/GlowtieV2/blob/main/images/GlowtieV2_Programming.jpg)

***
# 3D Printing
Depending on your application you want to print `Glowtie` or `Glowtie Slot`, both upside down with support enabled. The `Bumper` is optional, but will make it look better and offers protection.

![ButtonCase](https://github.com/KonradWohlfahrt/GlowtieV2/blob/main/images/GlowtieV2_Button.jpg)
_case for button_
![RibbonCase](https://github.com/KonradWohlfahrt/GlowtieV2/blob/main/images/GlowtieV2_Slot.jpg)
_case for ribbon_

***
# BLE Application
With Bluetooth Low Energy you need a dedicated application to change the characteristics.
I soon realized that learning a new framework takes a lot of time, so I opted for [MIT App Inventor](https://appinventor.mit.edu/).
Building your own app with this tool is easy and fast with the cost of less freedom when it comes to design.
I will come back and create a proper app with a framework like React Native or .Net Maui.
![ButtonCase](https://github.com/KonradWohlfahrt/GlowtieV2/blob/main/images/GlowtieV2_App.jpg)
_MIT App Inventor Application_

***
# Functions
The following buttons are used: upper button (Flash or B1) and lower button (B2).
Button functions:
- short press B1: show battery status
- short press B2: shuffle mode
- long press B1: save mode and color to eeprom
- long press B2: music mode
- double press B1: start BLE advertising (ble version) / swap to next effect (mic version)

![Glowtie](https://github.com/KonradWohlfahrt/GlowtieV2/blob/main/images/GlowtieV2.jpg)