
// Arduino IDE Board : Nologo ESP32C3 Super Mini     ( Tasmota : ESP32-C3 Dev Module )
// USB CDC on Boot   : Enabled                       ( for Serial Monitor )   
// Flash Mode        : QIO                           ( default, only Tasmota C3 needs DIO )

#include "IRremote.hpp"

const int irReceiverPin = 10;

void setup() {
  Serial.begin(115200);
  //while (!Serial);
  delay(4000);

  IrReceiver.begin(irReceiverPin , ENABLE_LED_FEEDBACK);  
 
  Serial.println("IRremote Receive Test !"); 
}

void loop(void)
{
      if (IrReceiver.decode()) {
        IrReceiver.printIRResultShort(&Serial);
        IrReceiver.resume();
      }
}

//Protocol=NEC Address=0x0 Command=0x45 Raw-Data=0xBA45FF00 32 bits LSB first

/*
Arduino FB

0x45  0x46  0x47
0x44  0x40  0x43
0x7   0x15  0x9
0x16  0x16  0xD  
      0x18
0x8   0x1C  0x5A
      0x52

*/
