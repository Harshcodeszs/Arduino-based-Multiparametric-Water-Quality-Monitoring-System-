# Arduino-based-Multiparametric-Water-Quality-Monitoring-System-
A simple Arduino project that reads 5 sensors simultaneously and sends data to Blynk app via ESP8266 for real-time monitoring.


5 Sensors → Arduino → ESP8266 → Blynk
What It Does
Reads 5 sensors at the same time and sends data to your phone via WiFi.

What You Need
Arduino (Uno/Nano)

ESP8266 (ESP-01)

5 sensors (temperature, humidity, etc.)

Jumper wires

Phone with Blynk app

Quick Setup
1. Wire Everything
ESP8266 to Arduino:

ESP TX → Arduino RX (Pin 0)

ESP RX → Arduino TX (Pin 1)

ESP VCC → Arduino 3.3V

ESP GND → Arduino GND

Sensors (example):

DHT11 → Pin 2

MQ-2 → A0

Ultrasonic → Pins 3 & 4

BMP280 → A4 & A5

MPU6050 → A4 & A5 (shared)

2. Install Libraries
In Arduino IDE: Sketch → Include Library → Manage Libraries
Install these:

Blynk

DHT sensor library

Adafruit BMP280

MPU6050_tockn

3. Set Up Blynk App
Download Blynk app

Create new project

Get Auth Token (sent to email)

Add 5 Value Display widgets

Set pins: V0, V1, V2, V3, V4

4. Upload This Code

#include <DHT.h>

// CHANGE THESE 3 LINES:
char auth[] = "your-token-here";
char ssid[] = "your-wifi-name";
char pass[] = "your-wifi-pass";

DHT dht(2, DHT11);

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200); // ESP8266
  dht.begin();
  delay(2000);
}

void loop() {
  // Read sensors
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int gas = analogRead(A0);
  
  // Send to Blynk
  sendToBlynk("V0", temp);
  sendToBlynk("V1", hum);
  sendToBlynk("V3", gas);
  
  delay(2000);
}

void sendToBlynk(String pin, float val) {
  Serial1.println(pin + ":" + String(val));
}

5. Test It
Open Serial Monitor (115200 baud)

See "WiFi Connected" message

Open Blynk app, tap play button

Watch sensor values update

Troubleshooting
No connection? Check WiFi password

Wrong readings? Check wiring

ESP not responding? Use 3.3V, not 5V`

Done!
Your sensors now sync and send data to your phone. Add more sensors by following the same pattern.
