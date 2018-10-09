#define USE_ETHERNET_SHIELD

#include "ThingSpeak.h"
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

EthernetClient client;
Adafruit_BMP085 bmp;

unsigned long myChannelNumber = 177086;
const char * myWriteAPIKey = "X00036HAX0WFQ1E5";

void setup() 
{
  //start the serial
  Serial.begin(9600);
  //network stuff
  Ethernet.begin(mac);
  ThingSpeak.begin(client);
  //start the sensor
  if (!bmp.begin()) 
  {
    Serial.println("Could not find BMP180 or BMP085 sensor at 0x77");
    while (1) {}
  }
}

void loop() 
{

  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  ThingSpeak.writeField(myChannelNumber, 1, bmp.readTemperature(), myWriteAPIKey);
  delay(20000); // ThingSpeak will only accept updates every 15 seconds.
}
