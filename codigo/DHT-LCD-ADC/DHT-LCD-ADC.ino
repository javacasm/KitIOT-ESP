/*
 * 
 * Placa ADC con PCB8591
 * https://arduinodiy.wordpress.com/2016/10/17/adding-a-pcf-8591-adcdac-to-esp8266-01/
 * 
 * Basado en el ejemplo de @Adafruit
// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// Depends on the following Arduino libraries:
// - Adafruit Unified Sensor Library: https://github.com/adafruit/Adafruit_Sensor
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
*/


/* ==== Includes ==== */
// Include para lcd
#include <Wire.h>
#include <LiquidCrystal_I2C_ESP.h>

// Includes para el sensor DHT
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>


/* ====  END Includes ==== */

/* ==== Defines ==== */

#define DHTPIN            D5         // Pin which is connected to the DHT sensor.

// Uncomment the type of sensor in use:
//#define DHTTYPE           DHT11     // DHT 11 
#define DHTTYPE           DHT22     // DHT 22 (AM2302)
//#define DHTTYPE           DHT21     // DHT 21 (AM2301)

#define SERIAL_BAUD 9600

/* ==== END Defines ==== */

/* ==== Global Variables ==== */
// Sensor DHT
DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

// LCD
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C_ESP lcd(0x27, 16, 2);


/* ==== END Global Variables ==== */


/* ==== Setup ==== */

void setup_Serie(){
  Serial.begin(9600);
}

void setup_DHT(){
   // Initialize device.
  dht.begin();
  Serial.println("DHTxx Unified Sensor Example");
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Temperature");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");  
  Serial.println("------------------------------------");
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Humidity");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");  
  Serial.println("------------------------------------");
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

void setup_LCD(){
    // initialize the LCD
  lcd.begin(0,2);  // sda=0 | D3, scl=2 | D4

  // Turn on the blacklight and print a message.
  lcd.backlight();

}

void setup() {
  setup_Serie();
  setup_DHT();
  setup_LCD();
}

/* ==== END Setup ==== */

/* ==== Loop ==== */

void loop() {
  // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.println(" *C");
    lcd.setCursor(0,0);
    lcd.print("T:");
    lcd.print(event.temperature);
    lcd.print("*C");
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
    lcd.setCursor(0,1);
    lcd.print("H:");
    lcd.print(event.relative_humidity);
    lcd.print("%");   
  }
}
/* ==== End Loop ==== */

/* ==== Functions ==== */
