
int LEDPin =13;

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LEDPin, HIGH);
  delay(1500);
  digitalWrite(LEDPin, LOW);
  delay(1000);
}
