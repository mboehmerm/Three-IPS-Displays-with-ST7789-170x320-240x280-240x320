//  Board Description : Wireless-Tag ESP32-P4 WT9932P4-TINY

//  Arduino IDE Board : ESP32P4 WT9932P4-TINY  or
//  Arduino IDE Board : ESP32P4 Dev Module 
//  USB CDC On Boot   : Enabled                (?)
//  Upload Mode       : UART0 / Hardware CDC   (Necessary for serial monitor on FUSB)
//  USB Mode          : Hardware CDC and JTAG  (Necessary for serial monitor on FUSB)


/*

There are no pin definitions for the RGB LED in the "ESP32P4 Dev Module". 
Adding them manually didn't help, so instead of digitalWrite() i had to use rgbLedWrite() or neopixelWrite().

But digitalWrite(LED_BUILTIN) works fine, if we use a custom board definition variant.
The file "boards.txt" has to be modified and "variants\esp32p4_wt9932p4_tiny\pins_arduino.h" has to be added :

AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.3.11\boards.txt                                     // modified
AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.3.11\variants\esp32p4_wt9932p4_tiny\pins_arduino.h  // added

Don't forget to backup your original "boards.txt".
Remember all changes are lost after Board Package esp32 3.3.11 is updated !!!

When we edit the file, we can move the most frequently used ones to the top. 
This ends the annoying scrolling that, according to Murphy's Law, is always at the very bottom.
Furthermore, you can set the options by changing the order of the entries.

Afterwards :
- Reload the board.txt in the Arduino IDE Menu : Tools/Reload Board Data
- Restart Arduino IDE

The new board definition contains the line
  esp32p4_wt9932p4_tiny.build.board=ESP32P4_WT9932P4_TINY
This can be determined by using "ARDUINO_" + "ESP32P4_WT9932P4_TINY"
  #ifdef ARDUINO_ESP32P4_WT9932P4_TINY
    //...
  #endif  

pins_arduino.h :

  #define PIN_RGB_LED 51

  // BUILTIN_LED can be used in new Arduino API digitalWrite() like in Blink.ino
  static const uint8_t LED_BUILTIN = SOC_GPIO_PIN_COUNT + PIN_RGB_LED;
  #define BUILTIN_LED LED_BUILTIN  // backward compatibility
  #define LED_BUILTIN LED_BUILTIN  // allow testing #ifdef LED_BUILTIN

  // RGB_BUILTIN and RGB_BRIGHTNESS can be used in new Arduino API rgbLedWrite()
  #define RGB_BUILTIN    LED_BUILTIN
  #define RGB_BRIGHTNESS 64

  .....
*/

//#define RGB_PIN LED_BUILTIN

#ifdef ARDUINO_ESP32P4_WT9932P4_TINY
  #define RGB_PIN LED_BUILTIN
  //#define RGB_PIN RGB_BUILTIN
  //#define RGB_PIN BUILTIN_LED
#else  
  #define RGB_PIN 51
#endif  

// ------------------------------------------------------------------------------------------ // 
void setup()
{
  Serial.begin(115200);
  //while (! Serial);
  
  //pinMode(RGB_PIN, OUTPUT);
}// ------------------------------------------------------------------------------------------ // 
void showPins()
{
  #ifdef ARDUINO_ESP32P4_WT9932P4_TINY
    Serial.println("Custom board definition : ESP32P4 WT9932P4-TINY");
    Serial.print  ("LED_BUILTIN: ");
    Serial.println(LED_BUILTIN);
    Serial.print  ("RGB_BUILTIN: ");
    Serial.println(RGB_BUILTIN);
  #endif
  Serial.print("BOOT_PIN: ");
  Serial.println(BOOT_PIN);
  
  // see FindOutSpiPins.ino
  Serial.print("MOSI: ");
  Serial.println(MOSI);
  Serial.print("MISO: ");
  Serial.println(MISO);
  Serial.print("SCK : ");
  Serial.println(SCK);
  Serial.print("SS  : ");
  Serial.println(SS);  
  Serial.print("SDA : ");
  Serial.println(SDA);  
  Serial.print("SCL : ");
  Serial.println(SCL);  
  Serial.print("RX  : ");
  Serial.println(RX);  
  Serial.print("TX  : "); 
  Serial.println(TX); 
  Serial.println(); 
}
// ------------------------------------------------------------------------------------------ // 
void blink(int pin, int n, int t) {
  for (int i=0; i<n; i++ ) {
    #ifdef ARDUINO_ESP32P4_WT9932P4_TINY
      digitalWrite(pin, HIGH);  
    #else  
      neopixelWrite(pin, 30, 30, 30);
    #endif  
    delay(t);
    #ifdef ARDUINO_ESP32P4_WT9932P4_TINY
      digitalWrite(pin, LOW);
    #else  
      neopixelWrite(pin,  0,  0,  0);
    #endif  
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

Output with ESP32P4 WT9932P4-TINY :

Custom board definition : ESP32P4 WT9932P4-TINY
LED_BUILTIN: 106
RGB_BUILTIN: 106
BOOT_PIN: 35
MOSI: 29
MISO: 31
SCK : 30
SS  : 28
SDA : 7
SCL : 8
RX  : 38
TX  : 37

Output with ESP32P4 Dev Module :

BOOT_PIN: 35
MOSI: 32
MISO: 33
SCK : 36
SS  : 26
SDA : 7
SCL : 8
RX  : 38
TX  : 37

*/
