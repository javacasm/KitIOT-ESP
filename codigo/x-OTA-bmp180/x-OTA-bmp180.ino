/*
 * To start mDNS monitor (OSX) exec:   dns-sd -B _arduino._tcp
 */

 // Wemos BMP180 shield https://github.com/wemos/D1_mini_Examples/blob/master/examples/03.Sensors/BMP180_BMP085/BMP180_BMP085.ino

#include "Config.h"   // Configuración del wifi

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;


#include <JeVe_EasyOTA.h>  // https://github.com/jeroenvermeulen/JeVe_EasyOTA/blob/master/JeVe_EasyOTA.h

#define ARDUINO_HOSTNAME "ota-wemos-caldera"

ESP8266WebServer server(80);

extern  char* ssid;
extern  char* password;

long blink_period;
long last_blink = millis();

long data_period;
long last_data = millis();


float temp, preassure, altitude;

String strTemp, strPrea, strAlt;

EasyOTA OTA;

void setup() {
  Serial.begin(9600);
  // This callback will be called when JeVe_EasyOTA has anything to tell you.
  OTA.onMessage([](char *message, int line) {
    Serial.println(message);
  });
  OTA.setup(ssid, password, ARDUINO_HOSTNAME);

  randomSeed(analogRead(0));
  blink_period=random(100,1000);

  pinMode(LED_BUILTIN,OUTPUT);

//  if (!bmp.begin()) {
//    Serial.println("Could not find BMP180 or BMP085 sensor at 0x77");
//    while (1) {
//      digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
//      Serial.println("Could not find BMP180 or BMP085 sensor at 0x77");
//      delay(50);
//     }
//  }

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
   server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", getRootPage());
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

String strPage;

const char * getRootPage(){
  strPage = strTemp + "<br>" +
                    strPrea + "<br>" +
                    strAlt;
  return strPage.c_str();
}

void handleRoot() {
  digitalWrite(LED_BUILTIN, HIGH);
  server.send(200, "text/plain", getRootPage());
  digitalWrite(LED_BUILTIN, LOW);
}

void handleNotFound(){
  digitalWrite(LED_BUILTIN, HIGH);
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
    digitalWrite(LED_BUILTIN, LOW);
}
// Get data from the sensor
void getData(){
  temp = bmp.readTemperature();
  preassure = bmp.readPressure();
  // Calculate altitude assuming 'standard' barometric
  // pressure of 1013.25 millibar = 101325 Pascal
  altitude =  bmp.readAltitude();
}

// Convert numerical to text data
void getStringData(){
  getData();
  strTemp = String("Temperatura = ") + temp + " °C";
  strPrea = String("Presion = ") + preassure + " Pa";
  strAlt = String("Altitud = ")+altitude + " metros";
}

// Show data over serial
void showData(){
  Serial.println(strTemp);
  Serial.println(strPrea);
  Serial.println(strAlt);
}

void loop() {
  
  // OTA
  OTA.loop();
  
  // Blink
  if(millis()-last_blink>blink_period){
    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
    last_blink=millis();
  }
  
  // Data
  if(millis()-last_data>data_period){
    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
    getStringData();
    showData();
    last_data=millis();
  }

  // Webserver
  server.handleClient();
  
}
