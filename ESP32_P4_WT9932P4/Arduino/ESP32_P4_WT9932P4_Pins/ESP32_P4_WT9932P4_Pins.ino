//  Board Description : Wireless-Tag ESP32-P4 WT9932P4-TINY

//  Arduino IDE Board : ESP32P4 Dev Module
//  USB CDC On Boot   : Enabled                (?)
//  Upload Mode       : UART0 / Hardware CDC   (Necessary for serial monitor on FUSB)
//  USB Mode          : Hardware CDC and JTAG  (Necessary for serial monitor on FUSB)

/*
// The pin definitions for RGB LED doesn't work.
// digitalWrite() doesn't work with pin 106

static const uint8_t PIN_RGB_LED = 51;
#define PIN_RGB_LED PIN_RGB_LED 

// BUILTIN_LED can be used in new Arduino API digitalWrite() like in Blink.ino
static const uint8_t RGB_BUILTIN = SOC_GPIO_PIN_COUNT + PIN_RGB_LED;             // 55+51=106
#define RGB_BUILTIN RGB_BUILTIN

// Define default brightness for the built-in RGB LED
#define RGB_BRIGHTNESS 64  // default brightness level (0-255)

// Define the color order for the built-in RGB LED
#define RGB_BUILTIN_LED_COLOR_ORDER LED_COLOR_ORDER_GRB  // default WS2812B color order

// Define the built-in LED pin
static const uint8_t LED_BUILTIN = 51;
#define LED_BUILTIN LED_BUILTIN  // allow testing #ifdef LED_BUILTIN
*/

#define RGB_PIN 51

// ------------------------------------------------------------------------------------------ // 
void setup()
{
  Serial.begin(115200);
  //while (! Serial);
  
  //pinMode(RGB_PIN, OUTPUT);
}// ------------------------------------------------------------------------------------------ // 
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
// ------------------------------------------------------------------------------------------ // 
void blink(int pin, int n, int t) {
  for (int i=0; i<n; i++ ) {
    //digitalWrite(pin, HIGH);  
    neopixelWrite(pin, 30, 30, 30);
    delay(t);
    //digitalWrite(pin, LOW);
    neopixelWrite(pin,  0,  0,  0);
    delay(200);
  }  
  delay(200);
}
// ------------------------------------------------------------------------------------------ //
void loop() {

  showPins();

  for (int r=64; r>=0; r=r-64)
    for (int g=64; g>=0; g=g-64)
      for (int b=64; b>=0; b=b-64)
        {
        rgbLedWrite(RGB_PIN, r, g, b);
        delay(1000);
        }
  delay(1000);

  blink(RGB_PIN, 3, 100);
  blink(RGB_PIN, 3, 400);
  blink(RGB_PIN, 3, 100);
  delay(2000);
}
// ------------------------------------------------------------------------------------------ //
/*

MOSI: 32
MISO: 33
SCK: 36
SS: 26
SDA: 7
SCL: 8
RX: 38
TX: 37
LED_BUILTIN: RGB_BUILTIN: 

*/
