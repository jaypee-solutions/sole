#include "TSIC.h"       // include the library
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// instantiate the library, representing the sensor
TSIC Sensor1(3, 2);    // Signalpin, VCCpin, Sensor Type
//TSIC Sensor1(4);    // only Signalpin, VCCpin unused by default

uint16_t temperature = 0;
float Temperatur_C = 0;
float temp_high_off = 24;
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
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.display();
  display.clearDisplay();
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

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Aussentemperatur:");
  display.setTextSize(2);
  display.println(Temperatur_C);
  display.setTextSize(1);
  display.println("");
  display.println("Pumpe:");
  display.setTextSize(2);
  if (currentPunmpState == pumpOff)
  {
    display.println("Aus");
  }
  else
  {
    display.println("An");
  }

  display.display();
  delay(switchTimer);


}

