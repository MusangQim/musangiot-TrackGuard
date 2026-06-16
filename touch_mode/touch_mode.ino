#include <Wire.h>
#include <Adafruit_GFS.h>
#include <Adafruit_SSD1306.h>

char currentState = "NORMAL";
int touchStartTime = 0;
int touchDebounceTime = 0;
bool touchHeld = false;

void setup() 
{
  
}

void loop() 
{
}
