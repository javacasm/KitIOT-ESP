// Wemos BMP180 shield https://github.com/wemos/D1_mini_Examples/blob/master/examples/03.Sensors/BMP180_BMP085/BMP180_BMP085.ino


#include "Wifi.h"

#include "WebServer.h"
#include "Data.h"

#include "Indicator.h"

long blink_period;
long last_blink = millis();

long data_period = 1000;
long last_data = millis();

void setup() {
  Serial.begin(9600);

  setup_indicator();

  setup_Wifi();
  
  // Para aleatorizar mas
  randomSeed(analogRead(0));

  // definimos el periodo de parpadeo
  blink_period=random(1000,2000);

  setup_Sensor();

  setup_webserver();
}



void loop() {

  loop_wifi();

  // Blink
  if(millis()-last_blink>blink_period){
    blinkIndicator();
    last_blink=millis();
  }

  // Data
  if(millis()-last_data>data_period){
    blinkIndicator();
    getData();
    formatData();
    showData();
    last_data=millis();
   // blinkIndicator();
  }

 loop_webserver();

}
