/*
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
#include "Config.h"   // Configuración del wifi

// Include para lcd
#include <Wire.h>
#include <LiquidCrystal_I2C_ESP.h>

// Includes para el sensor DHT
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Wifi
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
/* ====  END Includes ==== */

/* ==== Defines ==== */

#define DHTPIN            D5         // Pin which is connected to the DHT sensor.

// Uncomment the type of sensor in use:
//#define DHTTYPE           DHT11     // DHT 11
#define DHTTYPE           DHT22     // DHT 22 (AM2302)
//#define DHTTYPE           DHT21     // DHT 21 (AM2301)

#define SERIAL_BAUD 9600

#define PUERTO_SERVER 80

#define LED_WIFI D2

/* ==== END Defines ==== */

/* ==== Global Variables ==== */
// Sensor DHT
DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

// LCD
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C_ESP lcd(0x27, 16, 2);

// Wifi
extern const char* ssid;
extern const char* password;


ESP8266WebServer server(PUERTO_SERVER);

float fTemperatura,fHumedad;

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

void setup_led(){
  pinMode(LED_WIFI,OUTPUT);
}

void mensaje(String strMensaje){
  Serial.println(strMensaje);
  lcd.println(strMensaje);
}

void setup_wifi(){
  WiFi.begin(ssid, password);
  mensaje(String("Conectando wifi a ")+ssid);

  int iReintentos=0;
  while (WiFi.status() != WL_CONNECTED) {     // Wait for connection
    delay(1000);
    Serial.print(".");
    iReintentos++;
    if(iReintentos>50) {
      mensaje("Dejamos de intentarlo");
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    lcd.clear();
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    lcd.setCursor(3,1);
    lcd.print(WiFi.localIP());
    lcd.setCursor(10,1);
    lcd.print("IP:");

    if (MDNS.begin("esp8266")) {
      Serial.println("MDNS responder started");
    }

    server.on("/", handleRoot);

    server.on("/inline", [](){
      server.send(200, "text/plain", "this works as well");
    });

    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");
  } else {
    lcd.clear();
  }

}

void setup() {
  setup_Serie();
  setup_DHT();
  setup_LCD();
  setup_led();
  setup_wifi();
}

/* ==== END Setup ==== */

/* ==== Loop ==== */

void lee_Datos(){

    // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  fTemperatura=event.temperature;

  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  fHumedad=event.relative_humidity;
}

void show_Datos(){
    if (isnan(fTemperatura)) {
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.print("Temperature: ");
    Serial.print(fTemperatura);
    Serial.println(" *C");
    lcd.setCursor(0,0);
    lcd.print("T:");
    lcd.print(fTemperatura);
    lcd.print("*C");
  }

  if (isnan(fHumedad)) {
    Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(fHumedad);
    Serial.println("%  ");
    lcd.setCursor(0,1);
    lcd.print("H:");
    lcd.print(fHumedad);
    lcd.print("%  ");
  }
}

void loop() {
  // Delay between measurements.
  delay(delayMS);

  lee_Datos();

  show_Datos();
  // Wifi
  server.handleClient();
}
/* ==== End Loop ==== */

/* ==== Functions ==== */
void handleRoot() {
  digitalWrite(LED_WIFI, HIGH);
  char buffer[255];
  sprintf(buffer,"Temperatura: %d C\nHumedad: %d %\n",int(fTemperatura),int(fHumedad));
  server.send(200, "text/plain", buffer);
  digitalWrite(LED_WIFI, LOW);
}

void handleNotFound(){
  digitalWrite(LED_WIFI, HIGH);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(LED_WIFI, LOW);
}
