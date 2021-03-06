/*
 * To start mDNS monitor (OSX) exec:   dns-sd -B _arduino._tcp
 */
#include "Config.h"   // Configuración del wifi

#include <JeVe_EasyOTA.h>  // https://github.com/jeroenvermeulen/JeVe_EasyOTA/blob/master/JeVe_EasyOTA.h


#define ARDUINO_HOSTNAME "OTA-nodeMCU_robot_chasis"




long blink_period;

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
}
long last=millis();
void loop() {
  OTA.loop();
  if(millis()-last>blink_period){
    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
    last=millis();
  }

}
