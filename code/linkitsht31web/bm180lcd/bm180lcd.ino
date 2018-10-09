#include <LiquidCrystal.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
Adafruit_BMP085 bmp;

void setup() 
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);
  if (!bmp.begin()) 
  {
    Serial.println("Could not find BMP180 or BMP085 sensor at 0x77");
    while (1) {}
  }
}


void loop() 
{
  // set the cursor to column 0, line 0
  lcd.setCursor(0, 0);
  lcd.print("Temperature = " );
  lcd.print(bmp.readTemperature());
  // set the cursor to column 0, line 1
  lcd.setCursor(0, 1);
  lcd.print("Pressure = ");
  lcd.print(bmp.readPressure());
  delay(5000);
}

