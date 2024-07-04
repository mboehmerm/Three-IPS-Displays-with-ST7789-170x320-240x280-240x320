
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
