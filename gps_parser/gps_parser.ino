#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



void setup() 
{
  Serial.begin(9600);
  gpsSerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void parseGPRMC(line)
{
  char gpsBuffer[] = "$GPRMC,123519,A,3139.00,n,10168.69,E,   "

}

void updateOLED()
{
  char status;

  display.clearDisplay();
  if status == "A"
  {
    display.setCursor(,);
    display.println("LAT: ")
    display.setCursor(,);
    display.println("LON: ")
    display.setCursor(,);
    display.println("FIX: YES")
  }
}