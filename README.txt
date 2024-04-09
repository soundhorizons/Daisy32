ESP32 OSC Communication with Daisy Seed Patched via PlugData

CC0 LICENSE


This GitHub project provides a solution for establishing OSC (Open Sound Control) communication over Wifi between two ESP32 microcontrollers. 
The primary objective of this project is to facilitate data exchange between ESP32 devices while interfacing with the Daisy Seed synthesizer module patched with PlugData.

Key Features:

- OSC Communication: The project offers an implementation of OSC communication protocol, enabling data transmission between ESP32 microcontrollers.

- One-way Communication: An ESP32 device send OSC message to the other, then the receciver translates data with its 2 builtin Dac, to send on Daisy Seed board

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


Contributions to the project are highly encouraged. Whether it's bug fixes, feature enhancements, or documentation improvements.

Elevate your audio projects with the power of OSC communication between ESP32 devices and the Daisy Seed module patched with PlugData!
