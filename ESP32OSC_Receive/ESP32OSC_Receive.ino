#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <esp_task_wdt.h>

const char* ssid = "WifiName"; // Wifi network name
const char* password = "WifiPass"; // Wifi pass

// I connected devices with my phone hotspot, first printing on terminal the received IP address from the router, 
// then wrote it into the esp32 sender sketch code.
// I'd like to have a static IP assigned working if you can code it better

WiFiUDP Udp;
const unsigned int localPort = 8000; // OSC listening port

// samplerate parameters
const int sampleRate = 500; // Sample rate in Hz
unsigned long previousMicros = 0; // track last sample time

// Data array
const int maxValues = 6; // number of controls, fader, knobs, sensors etc
int values[maxValues]; // Array for control values
int ids[maxValues]; // Array for control ID

int j = 0; //samplerate counter loop

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password); // Connect to wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); // wait until it connects
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP()); // Print IP address

  Udp.begin(localPort); // Start UDP usage into specified port
  Serial.print("Listening port ");
  Serial.println(localPort);
}

void loop() {
  int size = Udp.parsePacket();
  if (size > 0) {
    OSCMessage msg;
    while (size--) {
      msg.fill(Udp.read()); // Fill OSCMessage with received data
    }
    
    if (!msg.hasError()) {
      // Make all your Functions to dispatch OSC messages
      msg.dispatch("/Message", handleMessage); // Handle your OSC message Based on your sent message
    } else {
      Serial.print("Error: ");
      Serial.println(msg.getError());
    }
  }

  unsigned long currentMicros = micros(); // Get Actual time in microseconds
  unsigned long timeInterval = 1000000 / sampleRate; // Interval between samples in microseconds

  if (currentMicros - previousMicros >= timeInterval) {
    previousMicros = currentMicros;

    // Send ID And Value to DAC
    dacWrite(25, values[j]);
    dacWrite(26, ids[j]);

    j = (j + 1) % maxValues;
  }

  // reset watchdog timer
  esp_task_wdt_reset();

}

// Make all your Functions to dispatch OSC messages
void handleMessage(OSCMessage& msg) {
  // Set below as your OSC message size, and content. I had been testing receiving: 
  // /Message followed by 2 integer values, one for control ID, the other from 0 to 255, ready for the 8 bit esp32 DAC
  if (msg.size() >= 2 && (msg.isInt(0) && msg.isInt(1))) {
    int ID = msg.getInt(0); // Axis ID
    int value = msg.getInt(1); // Mapped value

    //ID mapped for DAC, to be recognized ID+1>=RoundID>=ID into receiver
    int RoundID = static_cast<int>(round(((255.0 / 6) * ID) + (255.0 / 12))); 

    values[ID] = value; // Array for control values
    ids[ID] = RoundID; // Array for control ID

    //other tasks
  }
}
