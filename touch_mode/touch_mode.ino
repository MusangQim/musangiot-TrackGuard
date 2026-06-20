#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

char currentState[] = "NORMAL";
unsigned long touchStartTime = 0;
unsigned long touchDebounceTime = 0;
bool touchHeld = false;
bool touchFlag = false;
const byte touchPin = 4;

void IRAM_ATTR toggleISR()
{
  touchFlag = true;
}

void updateOLED()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  if (strcmp(currentState, "NORMAL") == 0)
  {
    display.setCursor(15, 25);
    display.println("NORMAL MODE");
  }
  else if (strcmp(currentState, "PANIC") == 0)
  {
    display.setCursor(15, 20);
    display.println("!! PANIC MODE !!");
  }
  else if (strcmp(currentState, "RESET") == 0)
  {
    display.setCursor(15, 25);
    display.println("RESET OK");
  }
  display.display();
}

void setup() 
{
  pinMode(touchPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(touchPin), toggleISR, FALLING);
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); //Loop forever, don proceed it
  }
  display.clearDisplay();
  display.display();
}

void loop()
{
  int touchState = digitalRead(touchPin);
  // Checking 3 second Hold for RESET
  if (touchState == HIGH)
  {
    // If start touch...
    if (!touchHeld)
    {
      touchHeld = true;
      touchStartTime = millis();
    }
    else if (millis() - touchStartTime > 3000)
    {
      strcpy(currentState, "RESET");
      updateOLED();
      delay(1000);
      strcpy(currentState, "NORMAL");
      updateOLED();
      touchHeld = false;
    }
  }
  else
  {
    // Finger release, reset held flag
    touchHeld = false;
  }
  if (touchFlag == true)
  {
    unsigned long timeNow = millis();
    if (timeNow - touchDebounceTime > 200)
    {
      touchDebounceTime = timeNow;
      if (strcmp(currentState, "NORMAL") == 0)
      {
        strcpy(currentState, "PANIC");
      }
      else if (strcmp(currentState, "PANIC") == 0)
      {
        strcpy(currentState, "NORMAL");
      }
      updateOLED();
    }
    touchFlag = false;
  }
}



