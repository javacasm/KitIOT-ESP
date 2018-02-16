#include <Adafruit_NeoPixel.h>


// https://github.com/wemos/D1_mini_Examples/blob/master/examples/04.Shields/WS2812B_RGB_Shield/simple/simple.ino
// https://wiki.wemos.cc/products:d1_mini_shields:ws2812b_rgb_shield
// https://wiki.wemos.cc/_media/products:d1_mini_shields:ws2812b_v1.0.0_1_16x9.jpg

#define PIN            D2


// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);



void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.


  int step=5;
  // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
  for (int r = 0; r < 250; r += step) {
    for (int g = 0; g < 250; g += step) {
      for (int b = 0; b < 250; b += step) {

        pixels.setPixelColor(0, pixels.Color(r , g, b)); // Moderately bright green color.

        pixels.show(); // This sends the updated pixel color to the hardware.

        delay(10); // Delay for a period of time (in milliseconds).
      }
    }
  }
}
