# Sole

This project is intended to be used as a temperature controller for a brine water heater/cooling.
It uses a TSIC temperature sensor and a 128x64 OLED display.

## Background

The air system of the house has a heat exchanger in front of the intake from the outside air.
This heat exchanger is connected to a long pipe filed with brine which is buried in the ground.
A pump circulates the brine through the pipe and the heat exchanger.
This controller is intended to control the pump based on the temperature of the outside air.
In summer it may help cooling the house, while in winter it reduces the energy needed to heat the house.

## IDE and dependencies

1. Download and install [Arduiono IDE](https://www.arduino.cc/en/Main/Software).
2. Start Arduino IDE and go to Library Manager icon on the left or Sketch -> Include Library -> Manage Libraries.
3. Search and install the following libraries:
  * Adafruit_SSD1306
  * Adafruit GFX (most like to already come with previous library)
  * TSIC
4. Open `sole.ino` in Arduino IDE.
5. Select board via Tools -> Board `Arduino Nano` or `Arduino Pro or Pro Mini` if flashing fails.
6. Select serial port via Tools -> Port.

## Parameters

``` c
float temp_high_off = 23; // upper limit where pump is turned off
float temp_low_off = 5.5; // lower limit where pump is turned off
```
