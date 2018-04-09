// include libraries
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "MotorShield.h"

#include "webServer.h"

#include "Config.h"

#include <JeVe_EasyOTA.h>  // https://github.com/jeroenvermeulen/JeVe_EasyOTA/blob/master/JeVe_EasyOTA.h


#define ARDUINO_HOSTNAME "OTA-nodeMCU_robot_4wd"


EasyOTA OTA;

// configure server

void setup(){
    Serial.begin(9600);
    // connect to wifi network
 /*   WiFi.begin(ssid, password);
    // static ip, gateway, netmask
    WiFi.config(IPAddress(192,168,1,155), IPAddress(192,168,1,1), IPAddress(255,255,255,0));
  */

    OTA.onMessage([](char *message, int line) {
      Serial.println(message);
    });
                                                                                    
    OTA.setup(ssid, password, ARDUINO_HOSTNAME);
  
    // connect
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(200);
        Serial.print(".");
    }
    
    Serial.println("Connectado");
    // set up the callback for http server
    setup_server();
    setup_motorShield();
}

void loop()
{
    // check for client connections
     severhandler();
     OTA.loop();
}
