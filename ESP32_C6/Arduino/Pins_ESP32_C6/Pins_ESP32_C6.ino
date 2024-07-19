
//  Board Description : nanoESP32-C6 Board    ( Aliexpress < 5€ )

//  Arduino IDE Board : ESP32C6 Dev Module
//  Flash Mode        : QIO                   ( default )
//  USB CDC On Boot   : Enabled               ( so serial monitor works )
//  USB Connector     : "ESP32C6"             ( not "CH343" )

// ------------------------------------------------------------------------------------------ // 
void setup()
{
  Serial.begin(115200);
  //while (! Serial);
  
  pinMode(39, OUTPUT);
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
}
/*
Output :

MOSI: 19
MISO: 20
SCK: 21
SS: 18
SDA: 23
SCL: 22
RX: 17
TX: 16
LED_BUILTIN: 39
*/
// ------------------------------------------------------------------------------------------ // 
void blink(int n, int t) {
  for (int i=0; i<n; i++ ) {
    digitalWrite(39, HIGH);  
    delay(t);
    digitalWrite(39, LOW);
    delay(200);
  }  
  delay(200);
}
// ------------------------------------------------------------------------------------------ //
void loop() {
int i;

  showPins();

  blink(3, 100);
  blink(3, 400);
  blink(3, 100);

  delay(2000);
}
// ------------------------------------------------------------------------------------------ //

