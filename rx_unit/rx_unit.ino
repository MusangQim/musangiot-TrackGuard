#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

// --- OLED ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- nRF24 ---
struct GPSPacket
{
  char lat[10];
  char lon[10];
  char status;
  char NS;
  char EW;
};
RF24 radio(4, 5); //(CE_Pin, CSN_Pin)
GPSPacket packet;
const byte address[6] = "00001";
unsigned long lastSignalTime = 0;

void setup()
{
  Serial.begin(115200);
  // --- +OLED ---
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println("SSD1306 failed");
    for(;;);
  }
  display.clearDisplay();
  display.display();
  // --- +NRF24 ---
  radio.begin
}

void loop()
{

}
