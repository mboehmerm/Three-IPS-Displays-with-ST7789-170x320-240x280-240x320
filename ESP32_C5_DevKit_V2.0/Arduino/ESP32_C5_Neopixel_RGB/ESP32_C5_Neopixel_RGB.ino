
// RGB_BUILTIN is the virtual pin 56, which is defined as 29+27=56 
// ESP32-C5 has 29 physical pins and the RGB LED is soldered to pin 27
// So functions like neopixelWrite() or digitalWrite() know how to handle the pin using NeoPixel protocols.

void setup() {
  // No pinMode or initialization required for neopixelWrite
}

void loop() {
  // Turn Red
  neopixelWrite(RGB_BUILTIN, 30, 0, 0);
  delay(1000);

  // Turn Green
  neopixelWrite(RGB_BUILTIN, 0, 30, 0);
  delay(1000);

  // Turn Blue
  neopixelWrite(RGB_BUILTIN, 0, 0, 30);
  delay(1000);

  // Turn Off
  neopixelWrite(RGB_BUILTIN, 0, 0, 0);
  delay(1000);
}
