// include libraries
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "MotorShield.h"

#include "Config.h"

// configure server
ESP8266WebServer server(80);
const char *form = "<center><form action='/'>"
"<button name='dir' type='submit' value='4'>Forward</button><br>"
"<button name='dir' type='submit' value='1'>Left</button> "
"<button name='dir' type='submit' value='2'>Right</button><br>"
"<button name='dir' type='submit' value='3'>Reverse</button><p>"
"<button name='dir' type='submit' value='5'>Stop</button>"
"</form></center>";

void handle_form()
{
    // only move if we submitted the form
    if (server.arg("dir"))
    {
        // get the value of request argument "dir"
        int direction = server.arg("dir").toInt();
        // chose direction
        switch (direction)
        {
            case 1:
                left();
                break;
            case 2:
                right();
                break;
            case 3:
                backward();
                break;
            case 4:
                forward();
                break;
            case 5:
                stop();
                break;
        }
        // move for 300ms, gives chip time to update wifi also
        delay(300);
    }

    // in all cases send the response
    server.send(200, "text/html", form);
}
void setup(){
    Serial.begin(9600);
    // connect to wifi network
    WiFi.begin(ssid, password);
    // static ip, gateway, netmask
    WiFi.config(IPAddress(192,168,1,155), IPAddress(192,168,1,1), IPAddress(255,255,255,0));
    // connect
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(200);
        Serial.print(".");
    }
    Serial.println("Connectado");
    // set up the callback for http server
    server.on("/", handle_form);
    // start the webserver
    server.begin();
    setup_motorShield();
}
void loop()
{
    // check for client connections
    server.handleClient();
}
