# Plataformas online

## Protocolos

### Rest API


![Ejemplo API Rest](https://static.techtalk.vn/wp-content/uploads/2016/04/RESTful-API-design-1014x457.jpg)

https://www.smashingmagazine.com/2018/01/understanding-using-rest-api/

https://www.prometec.net/esp8266-subir-valores/



### MQTT

[Botón MQTT](http://kaizoku.azurewebsites.net/2018/01/15/iot-button-part-1-arduino-mqtt-client)

![Arquitectura MQTT](./images/MQTT_arquitectura.png)

## Publicación


### Servidor propio

[Crear servidor en Raspberry](https://www.prometec.net/raspberry-pi-servidor/)

[Abrir servidor web a internet](https://www.prometec.net/raspberry-servidor-internet/)


#### Codigo


    WiFiClient client;
     const int httpPort = 80;
     if (!client.connect(host, httpPort)) {
     Serial.println("connection failed");
     return;
     }

     // We now create a URL for the request
     String url = "/dht11.php";
     String key = "?pass=1234";
     String dato1 = "&Temperatura=";
     String dato2 = "&Humedad=";

     Serial.print("Requesting URL: ");
     Serial.println(url);

     // This will send the request to the server
     client.print(String("GET ") + url + key + dato1 + temp + dato2 + hum + " HTTP/1.1\r\n" +
     "Host: " + host + "\r\n" +
     "Connection: close\r\n\r\n");
     unsigned long timeout = millis();
     while (client.available() == 0) {
     if (millis() - timeout > 5000) {
     Serial.println(">>> Client Timeout !");
     client.stop();
     return;
     }
     }
