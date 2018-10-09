#include <LiquidCrystal.h>
#include <Wire.h>
#include "Adafruit_HDC1000.h"

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
Adafruit_HDC1000 hdc = Adafruit_HDC1000();

void setup() 
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);
  if (!hdc.begin()) 
  {
    Serial.println("Couldn't find HDC1008 sensor!");
    while (1);
  }
}


void loop() 
{
  // set the cursor to column 0, line 0
  lcd.setCursor(0, 0);
  //display the temperature
  lcd.print("Temperature = " );
  lcd.print(hdc.readTemperature());
  
  // set the cursor to column 0, line 1
  lcd.setCursor(0, 1);
  //display the humidity
  lcd.print("Humidity = ");
  lcd.print(hdc.readHumidity());
  
  delay(1000);
}

