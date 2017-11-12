#include "TSIC.h"       // include the library
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

TSIC Sensor1(3, 2);    // Signalpin, VCCpin, Sensor Type
//TSIC Sensor1(4);    // only Signalpin, VCCpin unused by default

const int ledPin =  LED_BUILTIN;
const int buttonPin = 4;
const int pumpOn = LOW;
const int pumpOff = HIGH;
const int pumpPin = 10;
const int switchDelay = 900;

uint16_t temperature = 0;
float Temperatur_C = 0;
float temp_high_off = 24;
float temp_low_off = 7;
float histeresis = 0.5;

int ledState = LOW;
int buttonState = 0;
int currentPunmpState = pumpOff;
int targetPunmpState = pumpOff;
int switchTimer = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, pumpOff);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
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
  digitalWrite(ledPin, ledState);

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
    if (switchTimer >= switchDelay) {
      currentPunmpState = targetPunmpState;
      switchTimer = 0;
      digitalWrite(pumpPin, currentPunmpState);
    }
    else
    {
      switchTimer++;
    }
  } else {
    switchTimer = 0;
  }

  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    display.ssd1306_command(SSD1306_DISPLAYON);
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
    display.setTextSize(1);
    display.print(switchTimer);
    display.print("/");
    display.print(switchDelay);
    display.display();
  } else {
    display.ssd1306_command(SSD1306_DISPLAYOFF);
  }

  delay(1000);
  //
  //delay(switchTimer);
}

