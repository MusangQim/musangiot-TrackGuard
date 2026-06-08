#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

HardwareSerial gpsSerial(1);
char gLatitude[12] = "";
char gLongitude[12] = "";
char gNS = ' ';
char gEW = ' ';
char gStatus = 'V';
char gpsBuffer[100];
int buffIndex = 0;

void setup() 
{
  Serial.begin(9600);
  gpsSerial.begin(9600, SERIAL_8N1, 20, 21);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println("SSD1306 failed");
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();
  char testline[] = "$GPRMC,065733.00,A,0144.1500,N,10354.0200,E,0.0,0.0,080626,,,A*54";
  parseGPRMC(testline);
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
