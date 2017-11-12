# Sole

## Entwicklungsumgebung und Abhängigkeiten
1. Die [Arduiono IDE](https://www.arduino.cc/en/Main/Software) herunterladen und installieren.
2. Arduino IDE starten und über Sketch -> Bibliothek einbinden -> Bibliotheken verwalten den Bibliotheksverwalter starten.
3. Installieren der Bibliotheken
..* Adafruit_SSD1306
..* TSIC
..* Wire
..* SPI library
4. In der Arduiono IDE die `sole.ino` öffnen
5. Unter Werkzeuge -> Board `Arduino Nano` wählen
6. Unter Werkzeuge -> Port den Port des Arduion wählen

## Verstellparameter
```
float temp_high_off = 24;
float temp_low_off = 7;
```