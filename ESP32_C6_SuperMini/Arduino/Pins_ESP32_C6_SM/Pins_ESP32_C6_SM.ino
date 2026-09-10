
//  Board Description : ESP32-C6 Super Mini

//  Arduino IDE Board : MakerGO ESP32 C6 SuperMini  ( ESP32C6 Dev Module has different Pins ! )
//  Flash Mode        : QIO                         ( default )
//  USB CDC On Boot   : Enabled                     ( so serial monitor works )

#define LED_PIN 15
#define RGB_PIN 39   // So digitalWrite() works with the RGB_LED  

//#define RGB_PIN 8

// ------------------------------------------------------------------------------------------ // 
void setup()
{
  Serial.begin(115200);
  //while (! Serial);
  
  // Setting it to 0 makes serial writes non-blocking when no serial monitor is open, 
  // preventing lag or code hanging. This can be used only if USB CDC On Boot is enabled.
  Serial.setTxTimeoutMs(0);
  
  pinMode(LED_PIN, OUTPUT);
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

  blink(LED_PIN, 3, 100);
  blink(LED_PIN, 3, 400);
  blink(LED_PIN, 3, 100);
  delay(2000);

  blink(RGB_PIN, 3, 100);
  blink(RGB_PIN, 3, 400);
  blink(RGB_PIN, 3, 100);
  delay(2000);
}
// ------------------------------------------------------------------------------------------ //
/*
Output :

MakerGO ESP32 C6 SuperMini

MOSI: 5
MISO: 6
SCK: 7
SS: 4
SDA: 20
SCL: 19
RX: 17
TX: 16
LED_BUILTIN: 15
RGB_BUILTIN: 39

ESP32-C6 Dev Module

MOSI: 19
MISO: 20
SCK: 21
SS: 18
SDA: 23
SCL: 22
RX: 17
TX: 16
LED_BUILTIN: 39
RGB_BUILTIN: 39
*/
