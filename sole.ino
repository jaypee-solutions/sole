#include "TSIC.h"  // include the library
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define ONE_DAY_IN_SECONDS 86400


#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  // I2C Address of screen

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


TSIC Sensor1(3, 2);  // Signalpin, VCCpin, Sensor Type
//TSIC Sensor1(4);    // only Signalpin, VCCpin unused by default

const int ledPin = LED_BUILTIN;
const int buttonPin = 4;
const int pumpOn = LOW;
const int pumpOff = HIGH;
const int pumpPin = 10;
const int switchDelay = 1800; // prevent switching pump to frequently, wait at least 30 minutes before switching
unsigned long pumpSaferCounter = 0;

uint16_t temperature = 0;
float Temperatur_C = 10;
float temp_high_off = 23;
float temp_low_off = 5.5;
float histeresis = 0.5;

int ledState = LOW;
int buttonState = 0;
int currentPumpState = pumpOff;
int targetPumpState = pumpOff;
int switchTimer = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, pumpOff);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.display();
  display.clearDisplay();
}

void loop() {

  pumpSaferCounter++;

  if (currentPumpState == pumpOn)
    pumpSaferCounter = 0; // reset counter as long as pump is running


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
    targetPumpState = pumpOn;
  } else if (Temperatur_C >= temp_low_off && Temperatur_C <= (temp_high_off - histeresis)) {
    targetPumpState = pumpOff;
  } else if (Temperatur_C >= temp_high_off) {
    targetPumpState = pumpOn;
    switchTimer = switchDelay; // set switch timer to delay value to start immediately
  }

   if (pumpSaferCounter >= ONE_DAY_IN_SECONDS) {
    targetPumpState = pumpOn;
    switchTimer = switchDelay;
    pumpSaferCounter = 0;
   }

  if (targetPumpState != currentPumpState) {
    if (switchTimer >= switchDelay) {
      currentPumpState = targetPumpState;
      switchTimer = 0;
      digitalWrite(pumpPin, currentPumpState);
    } else {
      switchTimer++;
    }
  } else {
    switchTimer = 0;
  }

  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    display.ssd1306_command(SSD1306_DISPLAYON);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Aussentemp: ");
    display.println(Temperatur_C);
    display.drawCircle(105, 2, 2, WHITE);
    display.println("---------------------");
    display.print("Pumpe:      ");

    if (currentPumpState == pumpOff) {
      display.println("Aus");
    } else {
      display.println("An");
    }
    display.println("---------------------");
    display.print("Entprellen: ");
    display.println(secondsToHumanReadable(switchTimer));
    display.println("---------------------");
    display.print("Aus seit:   ");
    display.println(secondsToHumanReadable(pumpSaferCounter));
    display.display();
  } else {
    display.ssd1306_command(SSD1306_DISPLAYOFF);
  }

  delay(1000);
  //
  //delay(switchTimer);
}

String secondsToHumanReadable(unsigned long seconds) {
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  seconds %= 60;
  minutes %= 60;
  hours %= 24;
  return addLeadingZeroIfLessThanTen(hours) + ":" + addLeadingZeroIfLessThanTen(minutes) + ":" + addLeadingZeroIfLessThanTen(seconds);
}

String addLeadingZeroIfLessThanTen(unsigned long value) {
  if (value < 10){
    return "0" + String(value);
  }
  return String(value);
}
