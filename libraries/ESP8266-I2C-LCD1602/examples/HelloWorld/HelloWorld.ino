/*
       * ESP8266 with 16x2 I2C LCD
       * Compatible with the Arduino IDE 1.6.6
       * A fork from https://github.com/agnunez/ESP8266-I2C-LCD1602

       www.hobbytronics.com.pk
       https://blog.hobbytronics.com.pk
*/

#include <Wire.h>
#include <LiquidCrystal_I2C_ESP.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C_ESP lcd(0x27, 16, 2);

void setup()
{
	// initialize the LCD
	lcd.begin(0,2);  // sda=0 | D3, scl=2 | D4

	// Turn on the blacklight and print a message.
	lcd.backlight();
	lcd.print("Hello, world!");
}

void loop()
{
	// Do nothing here...
}
