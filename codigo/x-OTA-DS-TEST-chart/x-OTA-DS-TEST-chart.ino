#include "Config.h"   // Configuración del wifi

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <Wire.h>
#include <Adafruit_BMP085.h>

#include "CircularBuffer.h"

#include <OneWire.h>

OneWire  ds(D2);  // on pin D2 (a 4.7K resistor is necessary)


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

float temp;

String strTemp;

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

  String strBody = strTemp + "<br>";


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
  if(data->NData>0){
    int y = map(data->getValue(0),data->getMinimum(),data->getMaximum(),0,iHeight - iMargin) + iMargin / 2 ;
    float step = ( iWidth - 2.0 * iMargin ) / data->NData ;
    for (float x = iMargin, i = 1; i < data->NData; x += step, i++) {
      int y2 = map(data->getValue(i),data->minValue,data->maxValue,0,iHeight - iMargin) + iMargin / 2 ;
      sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", int(x), iHeight - y, int(x + step), iHeight - y2);
      out += temp;
      y = y2;
    }
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
 getTemp();
 TempData.addValue(temp);
}

void getTemp(){
  
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];

  
  if ( !ds.search(addr)) {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
  
  Serial.print("ROM =");
  for( i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
  }
  Serial.println();
 
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(100);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  Serial.print("  Data = ");
  Serial.print(present, HEX);
  Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print(" CRC=");
  Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  temp = (float)raw / 16.0;
  Serial.print("  Temperature = ");
  Serial.print(temp);
  Serial.println(" Celsius, ");
}

// Convert numerical to text data
void formatData(){
  strTemp = String("Temperatura = ") + temp + " C";
}

// Show data over serial
void showData(){
  Serial.println(strTemp);
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
