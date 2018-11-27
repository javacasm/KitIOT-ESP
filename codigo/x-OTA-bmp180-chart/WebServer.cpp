#include "CircularBuffer.h"

#include <ESP8266WebServer.h>

#define MAX_PAGE_LENGTH 1000

ESP8266WebServer server(80);

extern String strTemp, strPrea, strAlt;
extern CircularBuffer TempData;

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
    <br><a href=\"https://github.com/javacasm/Teleco_IOT/\">Codigo</a>\
    <br><img src=\"https://github.com/javacasm/Teleco_IOT/blob/master/images/Licencia_CC_peque.png?raw=true\">\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p>Medidas: %d</p>\
    <img src=\"/test.svg\" />\
  </body>\
</html>",
    strTitle.c_str(),strBody.c_str(),hr, min % 60, sec % 60,TempData.historicalCount
  );

  return pageBuffer;
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

String acercaDe(){
  return "<html>\
  <head>\
    <meta http-equiv='refresh' content='1'/>\
    <title>AcercaDe</title>\
    <style>\
      body { background-color: #aaaaaa; font-family:  Arial, Helvetica, Sans-Serif; font-size: 50px; Color: #011188; }\
    </style>\
  </head>\
  <body>\
    <br><a href=\"https://github.com/javacasm/Teleco_IOT/\">Codigo</a>\
    <br>Ejemplo de codigo HTML para nodeMCU\
  </body>\
</html>";
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

void setup_webserver(){
  server.on("/",  handleRoot);
  server.on("/test.svg", drawGraph );
  server.on("/about",acercaDe);
  server.on("/inline", [](){
    server.send(200, "text/plain", getRootPage());
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}


void loop_webserver(){
    // Webserver
  server.handleClient();
}
