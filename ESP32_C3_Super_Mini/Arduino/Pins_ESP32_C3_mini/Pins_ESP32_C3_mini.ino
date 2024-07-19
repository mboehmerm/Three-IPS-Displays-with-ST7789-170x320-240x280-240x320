
//  Arduino IDE Board : Nologo ESP32C3 Super mini oder ESP32C3 Dev Module
//  Flash Mode        : QIO                   ( default )
//  USB CDC On Boot   : Enabled               ( so serial monitor works )

// ------------------------------------------------------------------------------------------ // 
void setup()
{
  Serial.begin(115200);
  //while (! Serial);
  
  pinMode(8, OUTPUT);
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

Output Nologo ESP32C3 Super Mini :

MOSI: 6
MISO: 5
SCK: 4
SS: 7
SDA: 8
SCL: 9
RX: 20
TX: 21
LED_BUILTIN: 8

Output ESP32C3 Dev Module :

MOSI: 6
MISO: 5
SCK: 4
SS: 7
SDA: 8
SCL: 9
LED_BUILTIN: 30


Output LOLIN C3 MINI :

MOSI: 4
MISO: 3
SCK: 2
SS: 5
SDA: 8
SCL: 10
LED_BUILTIN: 7


Output LOLIN C3 PICO :

MOSI: 4
MISO: 0
SCK: 2
SS: 5
SDA: 8
SCL: 10
LED_BUILTIN: 7


Output ESP-C3-M1-I-Kit :
crasht




*/
// ------------------------------------------------------------------------------------------ // 
void blink(int n, int t) {
  for (int i=0; i<n; i++ ) {
    //digitalWrite(8, HIGH);  
    digitalWrite(8, LOW);        // LOW  macht die Led an  !
    delay(t);
    //digitalWrite(8, LOW);
    digitalWrite(8, HIGH);       // HIGH macht die Led aus !  
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

  delay(5000);
}
// ------------------------------------------------------------------------------------------ //

