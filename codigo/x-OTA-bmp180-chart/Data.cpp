#include <Wire.h>
#include <Adafruit_BMP085.h>
#include "CircularBuffer.h"

Adafruit_BMP085 bmp;

CircularBuffer TempData;

String strTemp, strPrea, strAlt;

float temp, preassure, altitude;

void setup_Sensor(){
    if (!bmp.begin()) {
    Serial.println("Could not find BMP180 or BMP085 sensor at 0x77");
    while (1) {
      digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
      Serial.println("Could not find BMP180 or BMP085 sensor at 0x77");
      delay(50);
     }
  }
}

// Get data from the sensor
void getData(){
  temp = bmp.readTemperature();
  preassure = bmp.readPressure();
  // Calculate altitude assuming 'standard' barometric
  // pressure of 1013.25 millibar = 101325 Pascal
  altitude =  bmp.readAltitude();

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
