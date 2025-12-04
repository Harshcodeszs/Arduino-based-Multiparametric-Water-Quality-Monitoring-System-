#include <OneWire.h>
#include <DallasTemperature.h>

const int SENSOR_PIN = 13;            // Arduino pin connected to DS18B20 sensor's DQ pin
const int TDS_SENSOR_PIN = A0;        // TDS sensor pin
const int TURBIDITY_SENSOR_PIN = A1; // Turbidity sensor pin
const int PH_SENSOR_PIN = A2;         // pH sensor pin 

OneWire oneWire(SENSOR_PIN);          // setup a oneWire instance
DallasTemperature tempSensor(&oneWire);// pass oneWire to DallasTemperature library

void setup() {
  Serial.begin(9600); // initialize serial communication
  tempSensor.begin(); // initialize the temperature sensor
}

void loop() {
  // Temperature sensor readings
  tempSensor.requestTemperatures();   // send the command to get temperatures
  float tempCelsius = tempSensor.getTempCByIndex(0); // read temperature in Celsius

  // TDS sensor readings
  float tdsValue = readTDS();

  // Turbidity sensor readings
  float turbidityVoltage = readSensorVoltage(TURBIDITY_SENSOR_PIN);

  // pH sensor readings
  float phValue = readPH();
  
  // Send sensor data over serial in specified format
  Serial.print("V5:");
  Serial.print(tempCelsius);
  Serial.print(" V6:");
  Serial.print(tdsValue);
  Serial.print(" V7:");
  Serial.print(turbidityVoltage);
  Serial.print(" V8:");
  Serial.println(phValue);
  
  delay(5); // Reduce delay for faster readings
}

float readTDS() {
   const float VREF = 5.0;
    const int SCOUNT = 30;
    int analogBuffer[SCOUNT];
    int analogBufferTemp[SCOUNT];
    int analogBufferIndex = 0;
    int copyIndex = 0;

    for (int i = 0; i < SCOUNT; i++) {
        analogBuffer[i] = analogRead(TDS_SENSOR_PIN);
        delay(10);
    }

    for (int i = 0; i < SCOUNT; i++)
        analogBufferTemp[i] = analogBuffer[i];

    for (int i = 0; i < SCOUNT - 1; i++) {
        for (int j = 0; j < SCOUNT - 1 - i; j++) {
            if (analogBufferTemp[j] > analogBufferTemp[j + 1]) {
                int temp = analogBufferTemp[j];
                analogBufferTemp[j] = analogBufferTemp[j + 1];
                analogBufferTemp[j + 1] = temp;
            }
        }
    }

    float averageVoltage = analogBufferTemp[SCOUNT / 2] * (float)VREF / 1024.0;
    float compensationCoefficient = 1.0 + 0.02 * (25.0 - 25.0);
    float compensationVoltage = averageVoltage / compensationCoefficient;
    float tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage - 255.86 * compensationVoltage * compensationVoltage + 857.39 * compensationVoltage) * 0.5;
    return tdsValue;
}

float readSensorVoltage(int pin) {
  int sensorValue = analogRead(pin); // Read the input on specified analog pin
  float voltage = sensorValue * (5.0 / 1024.0); // Convert the analog reading to a voltage (0 - 5V)
  return voltage; // Return the voltage value
}

float readPH() {
  const int numReadings = 10;
  int buf[numReadings];
  unsigned long int avgValue = 0;

  for (int i = 0; i < numReadings; i++) {
    buf[i] = analogRead(PH_SENSOR_PIN);
    delay(10);
  }

  // Sort the readings
  for (int i = 0; i < numReadings - 1; i++) {
    for (int j = i + 1; j < numReadings; j++) {
      if (buf[i] > buf[j]) {
        int temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }

  // Average the middle 6 readings
  for (int i = 2; i < numReadings - 2; i++) {
    avgValue += buf[i];
  }

  // Convert the average value to pH
  float voltage = avgValue * (5.0 / numReadings) / 1024.0;
  return 3.5 * voltage;
}
