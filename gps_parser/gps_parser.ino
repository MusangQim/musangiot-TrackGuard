#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HardwareSerial.h>
#include <cstring>

HardwareSerial gpsSerial(1);
char gLatitutde[12] = "";
char gLongitude[12] = "";
char gNS = ' ';
char gEW = ' ';
char gStatus = 'V';
char gpsBuffer[100];
int buffIndex = 0;

void setup()
{
  Serial.begin(9600);
  gpsSerial.begin(9600);
  char testline[] = "$GPRMC,065733.00,A,0144.1500,N,10354.0200,E,0.0,0.0,080626,,,A*54";
  parseGPRMC(tesline);
  updateOLED();
}

void loop() 
{
  while (gpsSerial.available() > 0)
  {
    char c = gpsSerial.read();
    if (c == '\n')
    {
      gpsBuffer[buffIndex] = '\0';
      if (strncmp(gpsBuffer, "$GPRMC", 6) == 0)
      {
        parseGPRMC(gpsBuffer);
        updateOLED();
      }
      buffIndex = 0;
    }
    else if (buffIndex < 99)
    {
      gpsBuffer[buffIndex++] = c;
    }
  }
}

void parseGPRMC(char line[])
{
  char copy[100];
  strcpy(copy, line);
  int fieldNum = 0;
  char *token = strtok(copy, ",");

  while (token != NULL)
  {
    if (fieldNum == 2)
      gStatus = token[0];
    if (fieldNum == 3)
      strcpy(gLatitude, token);
    if (fieldNum == 4)
      gNS = token[0];
    if (fieldNum == 5)
      strcpy(gLongitude, token);
    if (fieldNum == 6)
      gEW = token[0];
    fieldNum++;
    token = strtok(NULL, ",");
  }
}

void updateOLED()
{
  display.clearDisplay();
  if (gStatus == 'A')
  {
    display.setCursor(15,20);
    display.print("LAT: ");
    display.println(gLatitude);
    display.setCursor(15,35);
    display.print("LON: ");
    display.println(gLongitude);
    display.setCursor(15,50);
    display.println("FIX: YES");
  }
  else
  {
    display.setCursor(15, 20);
    display.println("FIX: NO");
  }
  display.display();
}