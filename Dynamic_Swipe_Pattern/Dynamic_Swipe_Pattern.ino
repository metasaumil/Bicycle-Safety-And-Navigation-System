int right[] = { 25, 33, 32 };
void setup() {
  // put your setup code here, to run once:
  pinMode(25, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(32, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(25, HIGH);
  delay(500);
  digitalWrite(33, HIGH);
  delay(500);
  digitalWrite(32, HIGH);
  delay(500);
  digitalWrite(25, LOW);
  delay(500);
  digitalWrite(33, LOW);
  delay(500);
  digitalWrite(32, LOW);
  delay(500);
}
