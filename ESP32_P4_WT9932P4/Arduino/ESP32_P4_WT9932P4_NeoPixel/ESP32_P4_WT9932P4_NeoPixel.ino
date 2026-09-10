
#include <Adafruit_NeoPixel.h>


// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        51 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS  1 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  delay(500);
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void showPins()
{
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
  Serial.print("RX: ");
  Serial.println(RX);  
  Serial.print("TX: ");
  Serial.println(TX); 
  Serial.print("LED_BUILTIN: ");
  //Serial.println(LED_BUILTIN);
  Serial.print("RGB_BUILTIN: ");
  //Serial.println(RGB_BUILTIN);
}

void loop() {

  showPins();

  int i = 0;
  pixels.setPixelColor(i, pixels.Color(128,   0,   0));
  pixels.show(); 
  delay(1000);
  pixels.setPixelColor(i, pixels.Color(  0, 128,   0));
  pixels.show(); 
  delay(1000);
  pixels.setPixelColor(i, pixels.Color(  0,   0, 128));
  pixels.show(); 
  delay(1000);

  pixels.clear(); // Set all pixel colors to 'off'
  for (int j = 0; j < 256; j=j+64) {
    //Serial.println(j);
    for (int k = 0; k < 256; k=k+64) {
      //Serial.println(k);
      for (int l = 0; l < 256; l=l+64) {
       // Serial.println(l);
        //Serial.println(j);
        for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...
          // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
          pixels.setPixelColor(i, pixels.Color(l, k, j));
          pixels.show();    // Send the updated pixel colors to the hardware.
          delay(250);       // Pause before next pass through loop
        }
      }
    }
  }
}
