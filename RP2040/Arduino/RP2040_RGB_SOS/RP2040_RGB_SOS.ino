/*
Menu File, Preferences...  "Additional Boards Manager URLs" field:
  https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json

Damit Pico als USB auftaucht :
  PicoBlink.ino.uf2 auf Laufwerk RPI-RP2 kopiert

Arduino IDE Board :  "VCC-GND YD-RP2040"

Flash Size: "2MB (no FS)" -> "16MB (Sketch 8MB, FS: 8MB)"
*/

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void blink(int n, int t) {
  for (int i=0; i<n; i++ ) {
    digitalWrite(LED_BUILTIN, HIGH);  
    delay(t);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }  
  delay(200);
}

void loop() {
int i;

  blink(3, 100);
  blink(3, 400);
  blink(3, 100);

  delay(2000);
}
