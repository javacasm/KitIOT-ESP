// Wemos BMP180 shield https://github.com/wemos/D1_mini_Examples/blob/master/examples/03.Sensors/BMP180_BMP085/BMP180_BMP085.ino

#include "Config.h"   // Configuración del wifi

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <Wire.h>
#include <Adafruit_BMP085.h>

#include "CircularBuffer.h"

//Adafruit_BMP085 bmp;


#include <JeVe_EasyOTA.h>  // https://github.com/jeroenvermeulen/JeVe_EasyOTA/blob/master/JeVe_EasyOTA.h

#define ARDUINO_HOSTNAME "OTA-wemos_bateria-test"

#define MAX_PAGE_LENGTH 1000

#define LED_INFO LED_BUILTIN

ESP8266WebServer server(80);

extern  char* ssid;
extern  char* password;

long blink_period;
long last_blink = millis();

long data_period = 1000;
long last_data = millis();

CircularBuffer TempData;

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

  pinMode(LED_INFO,OUTPUT);
/*
  if (!bmp.begin()) {
    Serial.println("Could not find BMP180 or BMP085 sensor at 0x77");
    while (1) {
      digitalWrite(LED_INFO,!digitalRead(LED_INFO));
      Serial.println("Could not find BMP180 or BMP085 sensor at 0x77");
      delay(50);
     }
  }
*/
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
    <p>Medidas: %d</p>\
    <p>Media: %f</p>\
    <img src=\"/test.svg\" />\
  </body>\
</html>",

    strTitle.c_str(),strBody.c_str(),hr, min % 60, sec % 60,TempData.historicalCount,TempData.getAverage()
  );

  return pageBuffer;
}



String getChart(CircularBuffer *data,String strColor){
  String out = "";
  char temp[100];
  int iWidth = 400 ;
  int iHeight = 140 ;
  int iMargin = 10 ;
  sprintf(temp , "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"%d\" height=\"%d\">\n",iWidth,iHeight);
  out += temp;
  sprintf(temp , "<rect width=\"%d\" height=\"%d\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n",iWidth,iHeight);
  out += temp; 
  out += "<g stroke=\""+strColor+"\">\n";
  int y = map(data->getValue(0),data->getMinimum(),data->getMaximum(),0,iHeight - iMargin) + iMargin / 2 ;
  float step = ( iWidth - 2.0 * iMargin ) / data->NData ;
  for (float x = iMargin, i = 1; i < data->NData; x += step, i++) {
    int y2 = map(data->getValue(i),data->minValue,data->maxValue,0,iHeight - iMargin) + iMargin / 2 ;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", int(x), iHeight - y, int(x + step), iHeight - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";
  return out;
}

void drawGraph() {
  

  server.send ( 200, "image/svg+xml", getChart(&TempData,"blue"));
}

void handleRoot() {
  digitalWrite(LED_INFO, HIGH);
  server.send(200, "text/HTML", getRootPage());
  digitalWrite(LED_INFO, LOW);
}

void handleNotFound(){
  digitalWrite(LED_INFO, HIGH);
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
    digitalWrite(LED_INFO, LOW);
}

// Get data from the sensor
void getData(){
/*  temp = bmp.readTemperature();
  
  preassure = bmp.readPressure();
  // Calculate altitude assuming 'standard' barometric
  // pressure of 1013.25 millibar = 101325 Pascal
  altitude =  bmp.readAltitude();
  */
  //temp = int(0.2 * random(10,30) + 0.8*TempData.getAverage());
  temp = random(10,30);
  preassure = random(1000,20000);
  altitude = random (0,700);
  TempData.addValue(int(temp*5));
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
    digitalWrite(LED_INFO,!digitalRead(LED_INFO));
    last_blink=millis();
  }
  
  // Data
  if(millis()-last_data>data_period){
    digitalWrite(LED_INFO,!digitalRead(LED_INFO));
    getData();
    formatData();
    showData();
    last_data=millis();
    digitalWrite(LED_INFO,!digitalRead(LED_INFO));
  }

  // Webserver
  server.handleClient();
  
}
