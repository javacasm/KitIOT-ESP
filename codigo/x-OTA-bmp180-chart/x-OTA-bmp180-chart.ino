// Wemos BMP180 shield https://github.com/wemos/D1_mini_Examples/blob/master/examples/03.Sensors/BMP180_BMP085/BMP180_BMP085.ino

#include "Config.h"   // Configuración del wifi

#include <ESP8266WiFi.h>
#include <WiFiClient.h>





#include "Data.h"
#include "WebServer.h"




#include <JeVe_EasyOTA.h>  // https://github.com/jeroenvermeulen/JeVe_EasyOTA/blob/master/JeVe_EasyOTA.h







extern  char* ssid;
extern  char* password;

long blink_period;
long last_blink = millis();

long data_period = 1000;
long last_data = millis();







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
  blink_period=random(1000,2000);

  pinMode(LED_BUILTIN,OUTPUT);

  setup_Sensor();

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  

  setup_webserver();
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
   // digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
  }

 loop_webserver();

}
