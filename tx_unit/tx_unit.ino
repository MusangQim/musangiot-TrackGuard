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

// ---GPS ---
HardwareSerial gpsSerial(1);
char gLatitude[12] = "";
char gLongitude[12] = "";
char gNS = ' ';
char gEW = ' ';
char gStatus = 'V';
char gpsBuffer[100];
int buffIndex = 0;


// --- Touch ---
char currentState[] = "NORMAL";
unsigned long touchStartTime = 0;
unsigned long touchDebounceTime = 0;
bool touchHeld = false;
bool touchFlag = false;
const byte touchPin = 0;

// --- nRF24 ---
struct GPSPacket
{
  char lat[10];
  char lon[10];
  char status;
  char NS;
  char EW;
};
RF24 radio(3, 7); //(CE_Pin, CSN_Pin)
GPSPacket packet;
const byte address[6] = "00001";

void IRAM_ATTR toggleISR()
{
  touchFlag = true;
}

void setup() 
{
  Serial.begin(115200);
  // --- +GPS ---
  gpsSerial.begin(9600, SERIAL_8N1, 20, 21);
  // --- +OLED ---
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println("SSD1306 failed");
    for(;;);
  }
  display.clearDisplay();
  display.display();
  // -- +TOUCH ---
  pinMode(touchPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(touchPin), toggleISR, CHANGE);
  // --- +NRF24
  radio.begin();
  radio.setChannel(100);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop()
{
  // --- +GPS ---
  while (gpsSerial.available() > 0)
  {
    char r = gpsSerial.read();
    Serial.print(r);
    if (r == '\n')
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
      gpsBuffer[buffIndex++] = r;
    }
  }
  // --- +TOUCH ---
  int touchState = digitalRead(touchPin);
  if (touchState == HIGH)
  {
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
  // --- +NRF24 ---
  strcpy(packet.lat, gLatitude);
  strcpy(packet.lon, gLongitude);
  packet.status = gStatus;
  packet.NS = gNS;
  packet.EW = gEW;
  bool result = radio.write(&packet, sizeof(packet));
  if (result)
  {
    Serial.println("[TX] Sent -> LAT:");
    Serial.println(packet.lat);
    Serial.println("LON:");
    Serial.println(packet.lon);
  }
  else
  {
    Serial.println("[TX] FAILED");
  }
  delay(1000);
}
