/*
File to C style array converter  

https://notisrac.github.io/FileToCArray/

Code format               Hex (0x00)
Treat as binary           NO

Palette mod               16bit RRRRRGGGGGGBBBBB
Resize                    NO  
Multi line                YES  
Optimize for column read  NO
Separate bytes of pixels  NO
Endianness                Little-Endian

static                    YES
const                     YES
unsigned                  NO
Data type                 uint16_t
PROGMEM                   YES

*/

#include <TFT_eSPI.h>                                            
TFT_eSPI tft = TFT_eSPI();                
   

#include "Flower_240x320.h"
#include "colorbands-320x240.h"
#include "Tv-test-pattern-146649_320X240.h"
#include "img_test_320_240.h"

void setup(void) {
  
  Serial.begin (115200);
  Serial.print ("ST7789 TFT Bitmap Test");

  //tft.begin ();                               // initialize a ST7789 chip
  tft.init ();                                  // initialize a ST7789 chip
  tft.setSwapBytes (true);                      // swap the byte order for pushImage() - corrects endianness

  tft.fillScreen (TFT_BLACK);
}

void loop() {

  tft.setRotation(0);
  tft.pushImage (0,0,240,320,Flower_240x320);
  delay(7000);
  
  tft.setRotation(1);
  tft.pushImage (0,0,320,240,colorbands_320x240);
  delay(7000);

  tft.setRotation(1);
  tft.pushImage (0,0,320,240,Tv_test_pattern_146649_320x240);
  delay(7000);

  tft.setRotation(1);
  tft.pushImage (0,0,320,240,img_test_320_240);
  delay(7000);

}