#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

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

void setup() 
{
  Serial.begin(9600);
  radio.begin();
  radio.setChannel(100);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop()
{
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
