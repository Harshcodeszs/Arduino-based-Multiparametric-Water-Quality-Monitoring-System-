#define BLYNK_TEMPLATE_ID "TMPL6E9KwK_pM"
#define BLYNK_TEMPLATE_NAME "Water Monitoring"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "7NVM-tj8lKtljl-6HxeP-PsTMwm6ROJ0"; // Blynk authentication token
char ssid[] = "Infinix NOTE 30 5G"; // Your WiFi SSID
char pass[] = "jieeeeee"; // Your WiFi password

ool sensorsEnabled = false; // Flag to indicate if sensors are enabled

void setup() {
  Serial.begin(9600); // Initialize serial communication
  Blynk.begin(auth, ssid, pass); // Connect to Blynk server
}b

void loop() {
  Blynk.run(); // Run Blynk
  
  if (Serial.available() > 0) { // Check if data is available to read
    String data = Serial.readStringUntil('\n'); // Read data from serial until newline character
    if (sensorsEnabled) {
      Blynk.virtualWrite(V5, atof(data.substring(data.indexOf("V5:") + 3, data.indexOf(",V6:")).c_str())); // Write temperature data to V5
      Blynk.virtualWrite(V6, atof(data.substring(data.indexOf("V6:") + 3, data.indexOf(",V7:")).c_str())); // Write TDS data to V6
      Blynk.virtualWrite(V7, atof(data.substring(data.indexOf("V7:") + 3, data.indexOf(",V8:")).c_str())); // Write turbidity data to V7
      Blynk.virtualWrite(V8, atof(data.substring(data.indexOf("V8:") + 3).c_str())); // Write pH data to V8
    } else {
      Blynk.virtualWrite(V5, 0); // Set temperature data to 0
      Blynk.virtualWrite(V6, 0); // Set TDS data to 0
      Blynk.virtualWrite(V7, 0); // Set turbidity data to 0
      Blynk.virtualWrite(V8, 0); // Set pH data to 0
    }
  }
}

// Function to handle button press event from Blynk app
BLYNK_WRITE(V9) {
  int buttonState = param.asInt(); // Read the state of the button (1 for pressed, 0 for released)
  
  if (buttonState == 1) { // If button is pressed
    sensorsEnabled = true; // Enable sensors
  } else { // If button is released
    sensorsEnabled = false; // Disable sensors
  }
}
