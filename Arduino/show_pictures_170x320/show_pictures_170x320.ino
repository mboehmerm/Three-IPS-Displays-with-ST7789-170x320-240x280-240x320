/*

Pictures are converted using File to C style array converter  
https://github.com/notisrac/FileToCArray

Code format               Hex (0x00)
Treat as binary           NO

Palette mod               16bit RRRGGGGGGBBBBB
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
   

#include "adabot170x320.h"
#include "Flower320x170.h"
#include "colorbands_small_320x170.h"
#include "Boat320x170.h"
#include "Field320x170.h"
#include "Balloon320x170.h"

void setup(void) {
  
  Serial.begin (115200);
  Serial.print ("ST7789 TFT Bitmap Test");

  tft.init ();                                  // initialize a ST7789 chip
  tft.setSwapBytes (true);                      // swap the byte order for pushImage() - corrects endianness

  tft.fillScreen (TFT_BLACK);
}

void loop() {

  tft.setRotation(2);
  tft.pushImage (0,0,170,320,adabot170x320);
  delay(3000);

  tft.setRotation(1);
  tft.pushImage (0,0,320,170,Flower320x170);
  delay(3000);
  
  tft.setRotation(1);
  tft.pushImage (0,0,320,170,colorbands_small_320x170);
  delay(3000);

  tft.setRotation(0);
  tft.pushImage (0,0,170,320,Boat320x170);
  delay(3000);

  tft.setRotation(0);
  tft.pushImage (0,0,170,320,Field320x170);
  delay(3000);

  tft.setRotation(0);
  tft.pushImage (0,0,170,320,Balloon320x170);
  delay(3000);

}