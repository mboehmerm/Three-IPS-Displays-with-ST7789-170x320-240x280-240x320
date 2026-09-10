
//  Board Description : ESP32-C5-DevKit V2.0

//  Arduino IDE Board : ESP32C5 Dev Module

#define RGB_PIN 56  // So digitalWrite() works with the RGB LED

// ------------------------------------------------------------------------------------------ // 
void setup()
{
  Serial.begin(115200);
  //while (! Serial);
  
  pinMode(RGB_PIN, OUTPUT);
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
  Serial.println(LED_BUILTIN);
  Serial.print("RGB_BUILTIN: ");
  Serial.println(RGB_BUILTIN);
}
// ------------------------------------------------------------------------------------------ // 
void blink(int pin, int n, int t) {
  for (int i=0; i<n; i++ ) {
    digitalWrite(pin, HIGH);  
    delay(t);
    digitalWrite(pin, LOW);
    delay(200);
  }  
  delay(200);
}
// ------------------------------------------------------------------------------------------ //
void loop() {
int i;

  showPins();

  blink(RGB_PIN, 3, 100);
  blink(RGB_PIN, 3, 400);
  blink(RGB_PIN, 3, 100);
  delay(2000);
}
// ------------------------------------------------------------------------------------------ //
/*
Output :

MakerGO ESP32 C5 Dev Module

ESP-ROM:esp32c5-eco2-20250121
Build:Jan 21 2025
rst:0x1 (POWERON),boot:0x18 (SPI_FAST_FLASH_BOOT)
SPI mode:DIO, clock div:1
load:0x408556b0,len:0x1258
load:0x4084bba0,len:0xcb4
load:0x4084e5a0,len:0x31a8
entry 0x4084bba0
E (105) MSPI Timing: Failed to allocate dummy cacheline for PSRAM memory barrier!
MOSI: 8
MISO: 9
SCK: 10
SS: 6
SDA: 0
SCL: 1
RX: 12
TX: 11
LED_BUILTIN: 56
RGB_BUILTIN: 56

*/
