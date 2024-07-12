ESP32 OSC Communication with Daisy Seed Patched via PlugData

CC0 LICENSE


This GitHub project provides a solution for establishing OSC (Open Sound Control) communication over Wifi between two ESP32 microcontrollers. 
The primary objective of this project is to facilitate data exchange using ESP32 while interfacing with the Daisy Seed synthesizer module patched with PlugData.

Key Features:

- OSC Communication: The project offers an implementation of OSC communication protocol.

- One-way Communication: An ESP32 device send OSC message to the other using GPIO as DACs, digital in/out, i2c DACs, PWM to analog signal using RC circuit.

- ESP32 Sketches: Included in the repository are two sketches for ESP32 microcontrollers. 
  These sketches are for OSC data handling and are well-commented for easy understanding and modification.

- PlugData Integration: The OSC data received from one ESP32 can be transmitted to the Daisy Seed synthesizer module patched with PlugData. 
  This integration expands the project's utility, allowing for real-time control and manipulation of audio parameters.

- Open Source: The entire project is open-source, encouraging collaboration and contributions from the community. Developers can modify and extend the codebase to suit their specific requirements.


Usage, recommendation:

- After you have projected a controller or prepared some sensors to use, update the basic example sketches and Plugdataìs patch, upload the respective ESP32 sketches to the devices using the Arduino IDE.

- Patch your Synthesizer with Plugdata, enable compile mode and flash your code into Daisy with Heavy Compiler Collection (hvcc)

- Ensure proper connectivity between the ESP32 devices and the Daisy Seed module, it Uses DAC on GPIO 25 for the value, GPIO 26 for ID value, put 2 pull down resistors between the connection of Daisy and ESP32.

- Be Sure to ground voltage Daisy and ESP32 receiver in the same place.

- Monitor the ESP 32 receiver IP address and OSC data exchange through the serial monitor into the Arduino IDE.

- Enjoy communication and control over the Daisy Seed synthesizer module via OSC messages.


components are powered through 5 volts wall adapter or portable battery, i've soldered an USB type c connector, and a voltage converter from 5 to 3.3v, to power everything from a single connector

components list:
- ESP32
- Daisy seed
- USB type c breakout connector
- x2 I2C Dacs(with different addresses)
- DC 5v to 3v3 step down module
- x3 jack stereo connectors
- for PWM 2 RC DACs, 4 condensers 22uF and 4 resistors 270 ohm
- pinstrips
- jumper cables

Below the table of GPIO connections between ESP32 and Daisy, and relative OSC messages assigned GPIOs

| ESP32 PIN   | DAISY PIN   | HVCCPARAM   | BUTTON     |
|:------------|:------------|:------------|:-----------|
| 13          | 26          | d1          | 1          |
| 12          | 27          | d2          | 2          |
| 27          | 8           | d3          | 3          |
| 14          | 9           | d4          | 4          |
| 33          | 10          | d5          | 5          |
| 32          | 11          | d6          | 6          |
| 0           | 5           | d7          | 7          |
| 15          | 6           | d8          | 8          |
| 4           | 7           | d9          | NOTE GATE  |

| ANALOG      | DAISY PIN   | HVCCPARAM   | FADER     | XY   | RADIAL     | RADIO    | NOTE CV   |
|:------------|:------------|:------------|:----------|:-----|:-----------|:---------|:----------|
| I2CDAC 1    | 17          | a1          | 1         | x1   | distance1  | Radio 1  | NOTE CV   |
| I2CDAC 2    | 18          | a2          | 2         | y1   | azimuth1   | Radio 2  |           |
| DAC 26      | 15          | a3          | 3         | x2   | distance2  | Radio 3  |           |
| DAC 25      | 16          | a4          | 4         | y2   | azimuth2   | Radio 4  |           |
| PWM 1 P5    | 19          | a5          | 5         | x3   | distance3  |          |           |
| PWM 2 P18   | 20          | a6          | 6         | y3   | azimuth3   |          |           |
| PWM 3 P19   | 21          | a7          | 7         | x4   | distance4  |          |           |
| PWM 4 P22   | 22          | a8          | 8         | y4   | azimuth4   |          |           |

OSC control messages names to write into osc interface to be dispatched from esp32:

| FADER     | BUTTON     | XY      | RADIAL     | RADIO     | NOTE CV to a1    |
|:----------|:-----------|:--------|:-----------|:----------|:-----------------|
| /fader1   | /button1   | /xy1    | /radial1   | /radio1   | /key1            |
| /fader2   | /button2   |         |            | /radio2   | /key2            |
| /fader3   | /button3   | /xy2    | /radial2   | /radio3   | /key3            |
| /fader4   | /button4   |         |            | /radio4   | /key4            |
| /fader5   | /button5   | /xy3    | /radial3   |           | /key5            |
| /fader6   | /button6   |         |            |           | /key6            |
| /fader7   | /button7   | /xy4    | /radial4   |           | /key7            |
| /fader8   | /button8   |         |            |           | /key8            |
|           |            |         |            |           | /key9            |
|           |            |         |            |           | /key10           |
|           |            |         |            |           | /key11           |
|           |            |         |            |           | /key12           |



Contributions to the project are highly encouraged. Whether it's bug fixes, feature enhancements, or documentation improvements.

Elevate your audio projects with the power of OSC communication between ESP32 devices and the Daisy Seed module patched with PlugData!
