
#include <Adafruit_NeoPixel.h>
 
#define PIN 23
 
// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

////       !!! Für RGB-LED beim RP2040 die Brücke RGB = R68 löten !!!

// ------------------------------------------------------------------------------------------ // 
void setup()
{
  pixels.begin(); // This initializes the NeoPixel library.
  Serial.begin(115200);

  //while (! Serial);
  delay(500);
  
  // see FindOutSpiPins.ino
  Serial.print("MOSI: ");
  Serial.println(MOSI);
  Serial.print("MISO: ");
  Serial.println(MISO);
  Serial.print("SCK: ");
  Serial.println(SCK);
  Serial.print("SS: ");
  Serial.println(SS);  
  Serial.print("SDA: ");
  Serial.println(SDA);  
  Serial.print("SCL: ");
  Serial.println(SCL);  
  Serial.print("LED_BUILTIN: ");
  Serial.println(LED_BUILTIN);
}
/*
Output :

MOSI: 19
MISO: 16
SCK: 18
SS: 17
SDA: 4
SCL: 5
LED_BUILTIN: 25
*/
// ------------------------------------------------------------------------------------------ // 
void show()
{

}
// ------------------------------------------------------------------------------------------ // 
void loop()
{
  int s0 = 4, s2 = 128, step = 4; 

  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.show();
  delay(2000);
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  pixels.show();
  delay(2000); 
  pixels.setPixelColor(0, pixels.Color(0, 0, 255));
  pixels.show();
  delay(2000); 
  pixels.setPixelColor(0, pixels.Color(255, 255, 0));
  pixels.show();
  delay(2000); 
  pixels.setPixelColor(0, pixels.Color(255, 0, 255));
  pixels.show();
  delay(2000); 
  pixels.setPixelColor(0, pixels.Color(0, 255, 255));
  pixels.show();
  delay(2000); 
  pixels.setPixelColor(0, pixels.Color(255, 255, 255));
  pixels.show();
  delay(5000); 

  pixels.setPixelColor(0, pixels.Color(127, 0, 0));
  pixels.show();
  delay(2000);
  pixels.setPixelColor(0, pixels.Color(0, 127, 0));
  pixels.show();
  delay(2000); 
  pixels.setPixelColor(0, pixels.Color(0, 0, 127));
  pixels.show();
  delay(2000); 
  pixels.setPixelColor(0, pixels.Color(127, 127, 0));
  pixels.show();
  delay(2000); 
  pixels.setPixelColor(0, pixels.Color(127, 0, 127));
  pixels.show();
  delay(2000); 
  pixels.setPixelColor(0, pixels.Color(0, 127, 127));
  pixels.show();
  delay(2000); 
  pixels.setPixelColor(0, pixels.Color(127, 127, 127));
  pixels.show();
  delay(5000); 


  // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
  for (int i = s0; i <= s2; i=i+step) {
    for (int j = s0; j <= s2; j=j+step) {
      for (int k = s0; k <= s2; k=k+step) {
        //pixels.setPixelColor(0, pixels.Color(i * 255, j * 255, k * 255)); // Moderately bright green color.
        pixels.setPixelColor(0, pixels.Color(i, j, k));
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(20); // Delay for a period of time (in milliseconds).
        Serial.print(".");
      }
    }
  }
  pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Moderately bright green color.
  pixels.show();
  delay(5000); 

  Serial.print("\n");
}
// ------------------------------------------------------------------------------------------ //

