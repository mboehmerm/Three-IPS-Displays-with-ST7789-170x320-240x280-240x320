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
   

#include "Flower_240x280.h"
#include "colorbands-280x240.h"
#include "Tv-test-pattern-146649_280X240.h"
#include "img_test_280_240.h"

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
  tft.pushImage (0,0,240,280,Flower_240x280);
  delay(7000);
  
  tft.setRotation(1);
  tft.pushImage (0,0,280,240,colorbands_280x240);
  delay(7000);

  tft.setRotation(1);
  tft.pushImage (0,0,280,240,Tv_test_pattern_146649_280x240);
  delay(7000);

  tft.setRotation(1);
  tft.pushImage (0,0,280,240,img_test_280_240);
  delay(7000);

}