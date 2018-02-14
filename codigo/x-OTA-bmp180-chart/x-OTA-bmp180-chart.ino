// Wemos BMP180 shield https://github.com/wemos/D1_mini_Examples/blob/master/examples/03.Sensors/BMP180_BMP085/BMP180_BMP085.ino

#include "Config.h"   // Configuración del wifi

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <Wire.h>
#include <Adafruit_BMP085.h>

#include "CircularBuffer.h"

Adafruit_BMP085 bmp;


#include <JeVe_EasyOTA.h>  // https://github.com/jeroenvermeulen/JeVe_EasyOTA/blob/master/JeVe_EasyOTA.h

#define ARDUINO_HOSTNAME "ota-wemos-caldera"

#define MAX_PAGE_LENGTH 1000

ESP8266WebServer server(80);

extern  char* ssid;
extern  char* password;

long blink_period;
long last_blink = millis();

long data_period = 500;
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

  // Para aleatorizar mas
  randomSeed(analogRead(0));

  // definimos el periodo de parpadeo
  blink_period=random(100,1000);

  pinMode(LED_BUILTIN,OUTPUT);

  if (!bmp.begin()) {
    Serial.println("Could not find BMP180 or BMP085 sensor at 0x77");
    while (1) {
      digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
      Serial.println("Could not find BMP180 or BMP085 sensor at 0x77");
      delay(50);
     }
  }

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
     server.on("/", handleRoot);
  server.on ( "/test.svg", drawGraph );
  server.on("/inline", [](){
    server.send(200, "text/plain", getRootPage());
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

char pageBuffer[MAX_PAGE_LENGTH];
String strTitle = "BMP180 Datos atmosfericos";
const char * getRootPage(){

  String strBody = strTemp + "<br>" +
                    strPrea + "<br>" +
                    strAlt;

 
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf ( pageBuffer, MAX_PAGE_LENGTH,

"<html>\
  <head>\
    <meta http-equiv='refresh' content='1'/>\
    <title>%s</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    %s\
    <p>Uptime: %02d:%02d:%02d</p>\
    <img src=\"/test.svg\" />\
  </body>\
</html>",

    strTitle.c_str(),strBody.c_str(),hr, min % 60, sec % 60
  );

  return pageBuffer;
}


CircularBuffer TempData;

void drawGraph() {
  String out = "";
  char temp[100];
  int iWidth = 400 ;
  int iHeight = 140 ;
  int iMargin = 10 ;
  sprintf(temp , "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"%d\" height=\"%d\">\n",iWidth,iHeight);
  out += temp;
  sprintf(temp , "<rect width=\"%d\" height=\"%d\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n",iWidth,iHeight);
  out += temp; 
  out += "<g stroke=\"black\">\n";
  int y = map(TempData.getValue(0),TempData.minValue,TempData.maxValue,0,iHeight - 10);
  int step = ( iWidth - 2 * iMargin ) / NData ;
  for (int x = iMargin, i = 0; x <= iWidth - iMargin; x += step, i++) {
    int y2 = map(TempData.getValue(i),TempData.minValue,TempData.maxValue,0,iHeight - iMargin);
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, iHeight - y, x + iMargin, iHeight - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";

  server.send ( 200, "image/svg+xml", out);
}

void handleRoot() {
  digitalWrite(LED_BUILTIN, HIGH);
  server.send(200, "text/HTML", getRootPage());
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
  server.send(404, "text/HTML", message);
    digitalWrite(LED_BUILTIN, LOW);
}

// Get data from the sensor
void getData(){
  temp = bmp.readTemperature();
  TempData.addValue(int(temp*5));
  preassure = bmp.readPressure();
  // Calculate altitude assuming 'standard' barometric
  // pressure of 1013.25 millibar = 101325 Pascal
  altitude =  bmp.readAltitude();
}

// Convert numerical to text data
void formatData(){
  strTemp = String("Temperatura = ") + temp + " C";
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
    getData();
    formatData();
    showData();
    last_data=millis();
    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
  }

  // Webserver
  server.handleClient();
  
}
