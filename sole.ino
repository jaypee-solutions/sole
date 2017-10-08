#include "TSIC.h"       // include the library

// instantiate the library, representing the sensor
TSIC Sensor1(4, 2);    // Signalpin, VCCpin, Sensor Type
//TSIC Sensor1(4);    // only Signalpin, VCCpin unused by default

uint16_t temperature = 0;
float Temperatur_C = 0;
float temp_high_off = 28;
float temp_low_off = 7;
float histeresis = 0.5;

const int ledPin =  LED_BUILTIN;// the number of the LED pin
const int pumpOn = LOW;
const int pumpOff = HIGH;
const int pumpPin = 10;

// Variables will change:
int ledState = LOW;             // ledState used to set the LED
int currentPunmpState = pumpOff;
int targetPunmpState = pumpOff;

int switchTimer = 900000;

void setup() {
  Serial.begin(9600); // set up the serial port
  pinMode(ledPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, pumpOff);
  switchTimer = 1000;
}

void loop() {
  if (Sensor1.getTemperature(&temperature)) {
    Temperatur_C = Sensor1.calc_Celsius(&temperature);
    Serial.print("Temperature: ");
    Serial.print(Temperatur_C);
    Serial.println(" °C");
  }
  if (ledState == LOW) {
    ledState = HIGH;
  } else {
    ledState = LOW;
  }

  if (Temperatur_C <= (temp_low_off - histeresis)) {
    targetPunmpState = pumpOn;
  }
  else if (Temperatur_C >= temp_low_off && Temperatur_C <= (temp_high_off - histeresis)) {
    targetPunmpState = pumpOff;
  }
  else if (Temperatur_C >= temp_high_off)
  {
    targetPunmpState = pumpOn;
  }

  if (targetPunmpState != currentPunmpState) {
    currentPunmpState = targetPunmpState;
    digitalWrite(pumpPin, currentPunmpState);
  }

  // set the LED with the ledState of the variable:
  digitalWrite(ledPin, ledState);

  delay(switchTimer);


}

