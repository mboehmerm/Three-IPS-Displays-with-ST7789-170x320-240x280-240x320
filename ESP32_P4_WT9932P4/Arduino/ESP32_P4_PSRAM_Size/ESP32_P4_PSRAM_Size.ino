/*

Arduino IDE Board  : "ESP32P4 Dev Module"
USB CDC On Boot    : "Enabled"
Chip Variant       : "Before v3.00"
PSRAM              : "Enabled"
Upload Mode        : "UART0 / Hardware CDC"
USB Mode           : "Hardware CDC and JTAG"

----------------------------------------------------

PSRAM successfully initialized!
Total PSRAM: 32 MB
Free PSRAM: 33551856 bytes
*/

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Check if PSRAM is enabled and working
  if (psramFound()) {
    Serial.println("PSRAM successfully initialized!");
    Serial.print("Total PSRAM: ");
    Serial.print(ESP.getPsramSize() / 1024 / 1024);
    Serial.println(" MB");
    
    Serial.print("Free PSRAM: ");
    Serial.print(ESP.getFreePsram());
    Serial.println(" bytes");
  } else {
    Serial.println("PSRAM not found or disabled in settings.");
  }
}

void loop() {
  // Nothing to do here
}