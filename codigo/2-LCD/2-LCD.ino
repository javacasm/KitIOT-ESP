

/* ==== Includes ==== */
// Include para lcd
#include <Wire.h>
#include <LiquidCrystal_I2C_ESP.h>




/* ====  END Includes ==== */

/* ==== Defines ==== */

#define SERIAL_BAUD 9600

/* ==== END Defines ==== */

/* ==== Global Variables ==== */

// LCD
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C_ESP lcd(0x27, 16, 2);


/* ==== END Global Variables ==== */


/* ==== Setup ==== */

void setup_Serie(){
  Serial.begin(9600);
}



void setup_LCD(){
    // initialize the LCD
  lcd.begin(0,2);  // sda=0 | D3, scl=2 | D4

  // Turn on the blacklight and print a message.
  lcd.backlight();

}

void setup() {
  setup_Serie();
  setup_LCD();
}

/* ==== END Setup ==== */

/* ==== Loop ==== */

void loop() {

  lcd.setCursor(0,0);
  lcd.print("Hola LCD");
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Adios LCD");
  lcd.clear();
}
/* ==== End Loop ==== */

/* ==== Functions ==== */
