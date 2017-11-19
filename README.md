# Sole

## Entwicklungsumgebung und Abhängigkeiten
1. Die [Arduiono IDE](https://www.arduino.cc/en/Main/Software) herunterladen und installieren.
2. Arduino IDE starten und über Sketch -> Bibliothek einbinden -> Bibliotheken verwalten den Bibliotheksverwalter starten.
3. Installieren der Bibliotheken
  * Adafruit_SSD1306
  * Adafruit GFX
  * TSIC
4. Adafruit_SSD1306.h auf Displaygröße anpassen 64 Pixel
5. In der Arduiono IDE die `sole.ino` öffnen
6. Unter Werkzeuge -> Board `Arduino Nano` wählen
7. Unter Werkzeuge -> Port den Port des Arduino wählen

## Verstellparameter
```
float temp_high_off = 24;
float temp_low_off = 7;
```