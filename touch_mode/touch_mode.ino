#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

char currentState[] = "NORMAL";
int touchStartTime = 0;
int touchDebounceTime = 0;
bool touchHeld = false;

const byte touchPin = 4;

void toggleISR()
{
  bool touchFlag = true;
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
}

void loop() 
{
  if (isr == true)
  {
    if (touchDebounceTime > 200)
    {
      
    }
  }
  if (digitalRead(touchPin, HIGH))
  {
    if > 3
    {
      current_state = "RESET";
    }
  }
}



