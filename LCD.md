# LCD I2C

Código más avanzado


    //Se incluyen las librerias
    #include <Wire.h>
    #include <LiquidCrystal_I2C.h>

    //Se define el comando printByte
    #define printByte(args)  write(args);
    //Se definen los caracteres personalizados
    uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
    uint8_t bell[8]  = {0x4,0xe,0xe,0xe,0x1f,0x0,0x4};

    //Se crea el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
    LiquidCrystal_I2C lcd(0x3F,16,2);  //

    void setup() {
      // Se inicializa el LCD
      lcd.init();

      //Seenciende la luz de fondo.
      lcd.backlight();
      lcd.createChar(0, heart);
      lcd.createChar(1, bell);

      // Se escribe el mensaje en el LCD en la fila 1, a partir de la columna 8
      lcd.setCursor(8, 0);
      lcd.printByte(0);
      lcd.print("Modulo 3");
      lcd.printByte(0);
      // Se escribe el mensaje en el LCD en la fila 2, a partir de la columna 5
      lcd.setCursor(5, 1);
      lcd.print("Ejercicio Tema 6..");
      lcd.printByte(1);
      lcd.printByte(1);
    }

    void loop() {
      //Se desplaza el texto una posición a la izquierda
      lcd.scrollDisplayLeft();
      delay(1000);
    }



## Librerías


https://github.com/agnunez/ESP8266-I2C-LCD1602

https://github.com/esp8266/Basic/blob/master/libraries/LiquidCrystal/LiquidCrystal_I2C.h


# Nokia 5110

https://kendrick-tabi.blogspot.com/2015/08/esp8266-and-nokia-5110-lcd.html
