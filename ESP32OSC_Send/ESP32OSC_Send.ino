#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>


const char* ssid = "WifiName"; // Your Wifi network name
const char* password = "WifiPass"; // Your Wifi password


// Data arrays
const int maxValues = 4; // number of controls, fader, knobs, sensors etc
uint8_t Data[maxValues]; // Control data, at the end I've mapped it from 0 to 255 ready for the 8 bit Esp32 receiver's DAC
uint8_t oldvalue[maxValues] = {255, 255, 255, 255};
uint8_t  pinPots[maxValues] = {34, 35, 27, 33}; // I've filled this with com ESP32 Analog pins for example, to read below in a for loop

WiFiUDP Udp;
const unsigned int port = 8000; // OSC port to send messages

const char* outIp = "192.168.236.138"; // your receiver IP

void setup() {
  WiFi.begin(ssid, password); // Connect to wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); // wait until it connects
  }
  Udp.begin(port); // Start UDP usage into specified port

}

void loop() {

  // Do your magic here: analog read, i2c, sensors etc
  
  // For example read some knobs
  for (int i = 0; i < maxValues; i++) 
  { 
    float rawValue = analogRead(pinPots[i]); // Read Analog pins declared on the top
    // Data[i] = map(rawValue, 0, 16385, 0, 255); // Remap your reading for 8 bit DAC
  }

  for (int i = 0; i < maxValues; i++) 
  {
    if (oldvalue[i] != Data[i]) // I've used this to dont send many repetitive osc message data
    {
      OSCMessage msg("/GY521"); // Create an OSC Message with a specified name
      msg.add(i); // Add an ID identifier value to the message
      msg.add(Data[i]); // Add a value into your specified ID control
  
      Udp.beginPacket(outIp, port); // Create a packet destined to the specified IP and port
      msg.send(Udp); // Send Message through UDP
      Udp.endPacket(); // Complete package sending

      oldvalue[i] = Data[i]; // Updata last received value

      delay(4); //use delay at your own discretion to appropriately send timed data
    }
  }  
  
}
