#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <esp_task_wdt.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <Control_Surface.h>

// Create an instance of the class MCP4725 for each DAC
Adafruit_MCP4725 dac1;
Adafruit_MCP4725 dac2; 

#define I2C_SDA 21
#define I2C_SCL 22

// Configure the MIDI interface on HardwareSerial (ESP32)
HardwareSerialMIDI_Interface midi = {Serial2, MIDI_BAUD}; //useful library for future midi implementations

// Network credentials
char ssid[] = "ESP32_AP";      // WiFi network name, ESP32 create its own network and then connect ddevices to it
char password[] = "123456789"; // WiFi password
//when you upload the sketch, watch for the esp32 ip address in the serial monitor to connect other devices, usually is 192.168.4.1

// UDP configuration
WiFiUDP Udp;
const unsigned int localPort = 8000; // Local port to listen for UDP packets

const int blockdelay = ((1 / 48000) * 48 * 1000000 * 1); //48 samples at 48khz in microseconds, * N, ajust for your Daisy blocksize

const int DigitalOuts[9] = {13, 12, 14, 27, 33, 32, 0, 15, 4};
            // daisy gpio   27, 26, 8, 9, 10, 11, 5, 6, 7

int velocity;  //values for keyboard touch control
int CV[8]; //assign to touch faders, knobs etc

const int pwmPins[4] = {5, 18, 19, 23};
const int numPwmChannels = 4; // Number of PWM channels


void setup() {
  Serial.begin(115200);
  for(int i = 0; i < 9; i++)
  {
    pinMode(DigitalOuts[i], OUTPUT);
    digitalWrite(DigitalOuts[i], HIGH); // Gate, pulse/toggle
  }
  for(int i = 0; i < numPwmChannels; i++)
  {
    pinMode(pwmPins[i], OUTPUT);
    ledcAttach(pwmPins[i], 1000, 8); // Configure and attach pin to PWM channel
  }
  Wire.begin(I2C_SDA, I2C_SCL); // Start the I2C bus

  // Initialize each DAC with its own I2C address
  dac1.begin(0x60); // Address of the first DAC
  dac2.begin(0x61); // Address of the second DAC

  // Configure ESP32 as access point
  WiFi.softAP(ssid, password);

  delay(100);

  // Get IP address
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(IP);

  // Start UDP server
  Udp.begin(localPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", IP.toString().c_str(), localPort);

  // Define the configuration structure
  esp_task_wdt_config_t wdt_config = {
      .timeout_ms = 5000,    // Set timeout to 5000 milliseconds (5 seconds)
      .idle_core_mask = 1,   // Monitor core 0's idle task
      .trigger_panic = true  // Trigger panic when timeout occurs
  };
  esp_task_wdt_add(NULL);     // Add the current task
}

void loop() {
  bool ReadDaisy;

  int size = Udp.parsePacket();
  if (size > 0) {
    OSCMessage msg;
    while (size--) {
      msg.fill(Udp.read());
    }

    if (!msg.hasError()) {

      msg.dispatch("/fader1", fader1);
      msg.dispatch("/fader2", fader2);
      msg.dispatch("/fader3", fader3);
      msg.dispatch("/fader4", fader4);
      msg.dispatch("/fader5", fader5);
      msg.dispatch("/fader6", fader6);
      msg.dispatch("/fader7", fader7);
      msg.dispatch("/fader8", fader8);

      msg.dispatch("/button1", toggle1);
      msg.dispatch("/button2", toggle2);
      msg.dispatch("/button3", toggle3);
      msg.dispatch("/button4", toggle4);
      msg.dispatch("/button5", toggle5);
      msg.dispatch("/button6", toggle6);
      msg.dispatch("/button7", toggle7);
      msg.dispatch("/button8", toggle8);

      msg.dispatch("/key1", key1);
      msg.dispatch("/key2", key2);
      msg.dispatch("/key3", key3);
      msg.dispatch("/key4", key4);
      msg.dispatch("/key5", key5);
      msg.dispatch("/key6", key6);
      msg.dispatch("/key7", key7);
      msg.dispatch("/key8", key8);
      msg.dispatch("/key9", key9);
      msg.dispatch("/key10", key10);
      msg.dispatch("/key11", key11);
      msg.dispatch("/key12", key12);

      msg.dispatch("/xy1", xy1);
      msg.dispatch("/xy2", xy2);
      msg.dispatch("/xy3", xy3);
      msg.dispatch("/xy4", xy4);

      msg.dispatch("/radar1", radar1);
      msg.dispatch("/radar2", radar2);
      msg.dispatch("/radar3", radar3);
      msg.dispatch("/radar4", radar4);

      msg.dispatch("/radio1", radio1);
      msg.dispatch("/radio2", radio2);
      msg.dispatch("/radio3", radio3);
      msg.dispatch("/radio4", radio4);

    } else {
      Serial.print("OSC Error: ");
      Serial.println(msg.getError());
    }
  }


  
  
  dac1.setVoltage(CV[0], false); // Map note to CV

  digitalWrite(DigitalOuts[8], !velocity); //Velocity gate to pin 32 if touch keyboard is used

  dac2.setVoltage(CV[1], false);
  int dac3 = CV[2] / 16;
  int dac4 = CV[3] / 16;
  dacWrite(26, dac3); //calculated values in OSC functions in 12 bits, reducing to 8 bits for ESP32 DACs
  dacWrite(25, dac4);
  

  for(int i = 0; i < numPwmChannels; i++)
  {
    int cv_index = i + 4;
    int PWManalog = CV[cv_index] / 16 ;
    // Set the PWM value on the specified channel
    ledcWrite(pwmPins[i], PWManalog);
  }

  delayMicroseconds(blockdelay);

  esp_task_wdt_reset(); // Reset the watchdog timer
}

//FADER
void fader1(OSCMessage& msg) {
  handleFader(msg, 0);
}
void fader2(OSCMessage& msg) {
  handleFader(msg, 1);
}
void fader3(OSCMessage& msg) {
  handleFader(msg, 2);
}
void fader4(OSCMessage& msg) {
  handleFader(msg, 3);
}
void fader5(OSCMessage& msg) {
  handleFader(msg, 4);
}
void fader6(OSCMessage& msg) {
  handleFader(msg, 5);
}
void fader7(OSCMessage& msg) {
  handleFader(msg, 6);
}
void fader8(OSCMessage& msg) {
  handleFader(msg, 7);
}

//XY
void xy1(OSCMessage& msg) {
  xydata(msg, 0, 1);
}
void xy2(OSCMessage& msg) {
  xydata(msg, 2, 3);
}
void xy3(OSCMessage& msg) {
  xydata(msg, 4, 5);
}
void xy4(OSCMessage& msg) {
  xydata(msg, 6, 7);
}

//RADAR
void radar1(OSCMessage& msg) {
  radardata(msg, 0, 1);
}
void radar2(OSCMessage& msg) {
  radardata(msg, 2, 3);
}
void radar3(OSCMessage& msg) {
  radardata(msg, 4, 5);
}
void radar4(OSCMessage& msg) {
  radardata(msg, 6, 7);
}

//PULSE
void toggle1(OSCMessage& msg) {
    toggle(msg, 0);
}
void toggle2(OSCMessage& msg) {
    toggle(msg, 1);
}
void toggle3(OSCMessage& msg) {
    toggle(msg, 2);
}
void toggle4(OSCMessage& msg) {
    toggle(msg, 3);
}
void toggle5(OSCMessage& msg) {
    toggle(msg, 4);
}
void toggle6(OSCMessage& msg) {
    toggle(msg, 5);
}
void toggle7(OSCMessage& msg) {
    toggle(msg, 6);
}
void toggle8(OSCMessage& msg) {
    toggle(msg, 7);
}
void toggle9(OSCMessage& msg) {
    toggle(msg, 8);
}

//RADIO
void radio1(OSCMessage& msg) {
  radiodata(msg, 0);
}
void radio2(OSCMessage& msg) {
  radiodata(msg, 1);
}
void radio3(OSCMessage& msg) {
  radiodata(msg, 2);
}
void radio4(OSCMessage& msg) {
  radiodata(msg, 3);
}

//KEYS
void key1(OSCMessage& msg) {
    PVkey(msg, 0);
}
void key2(OSCMessage& msg) {
    PVkey(msg, 1);
}
void key3(OSCMessage& msg) {
    PVkey(msg, 2);
}
void key4(OSCMessage& msg) {
    PVkey(msg, 3);
}
void key5(OSCMessage& msg) {
    PVkey(msg, 4);
}
void key6(OSCMessage& msg) {
    PVkey(msg, 5);
}
void key7(OSCMessage& msg) {
    PVkey(msg, 6);
}
void key8(OSCMessage& msg) {
    PVkey(msg, 7);
}
void key9(OSCMessage& msg) {
    PVkey(msg, 8);
}
void key10(OSCMessage& msg) {
    PVkey(msg, 9);
}
void key11(OSCMessage& msg) {
    PVkey(msg, 10);
}
void key12(OSCMessage& msg) {
    PVkey(msg, 11);
}

//FADER
void handleFader(OSCMessage& msg, int faderID) {

  float val = msg.getFloat(0);

  CV[faderID] = static_cast<int>(round(val * 4095));

  Serial.print("\nfader ");
  Serial.print(faderID + 1);
  Serial.print(", value:");
  Serial.print(CV[faderID]);
  Serial.print("\n");

}
//TOGGLE
void toggle(OSCMessage& msg, int IDgate) {

  int gate = static_cast<int>(msg.getFloat(0));
    
  digitalWrite(DigitalOuts[IDgate], !gate);

  Serial.print("\nReceived gate value: ");
  Serial.print(gate);
  Serial.print(" - Setting pin: "); 
  Serial.print(DigitalOuts[IDgate]);
  Serial.print(" to: ");
  Serial.print(!gate);
  Serial.print("\n");

}

//XY
void xydata(OSCMessage& msg,int xid, int yid){

  float valxy;
    
  valxy = msg.getFloat(0); // X

  CV[xid] = static_cast<int>(round(valxy * 4095));

  Serial.print("\nDAC: ");
  Serial.print(xid);
  Serial.print(", x: ");
  Serial.print(CV[xid]);


  valxy = msg.getFloat(1); // Y

  CV[yid] = static_cast<int>(round(valxy * 4095));

  Serial.print("\nDAC:");
  Serial.print(yid);
  Serial.print(", y: ");
  Serial.print(CV[yid]);
  Serial.print("\n");

}

//RADAR
void radardata(OSCMessage& msg,int aid, int did){

  float valad;

  valad = msg.getFloat(0); // Distance from the center

  CV[aid] = static_cast<int>(round(valad * 4095)); 

  Serial.print("\nDAC: ");
  Serial.print(aid);
  Serial.print(", x: ");
  Serial.print(CV[aid]);


  valad = msg.getFloat(1); // Azimuth

  CV[did] = static_cast<int>(round(valad * 4095)); 

  Serial.print("\nDAC: ");
  Serial.print(did);
  Serial.print(", y: ");
  Serial.print(CV[did]);
  Serial.print("\n");
  
}

//RADIO
void radiodata(OSCMessage& msg,int buttonid) {

  int bnumber = msg.getInt(0);
  CV[buttonid] = (((4095 / 8) * bnumber) + (4095 / (2 * 8)));
  
  Serial.print("\nDAC: ");
  Serial.print(buttonid) ;
  Serial.print(", value: ");
  Serial.print(CV[buttonid]) ;
  Serial.print("\n");

}

//KEYS
void PVkey(OSCMessage& msg,int pitch) {
  velocity = static_cast<int>(msg.getFloat(0));
  CV[0] = (((4095 / 12) * pitch) + (4095 / (2 * 12)));
  
  Serial.print("\nDACpitch: ");
  Serial.print(CV[0]) ;
  Serial.print(" pin: ");
  Serial.print(DigitalOuts[8]);
  Serial.print(", !vel:");
  Serial.print(!velocity);
  Serial.print("\n");
}

