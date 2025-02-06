//arduino

#include <Wire.h>

#define I2C_SLAVE_ADDR 8  // Same address as in ESP32

volatile int receivedSpeed = 0;
int a = 13, b = 12, c = 11, d = 10, e = 9, f = 8, g = 7, a2 = 6, b2 = 5, c2 = 4, d2 = 3, e2 = 2, f2 = 1, g2 = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SLAVE_ADDR);
  Wire.onReceive(receiveEvent);
  for(int i = 0; i<=13; i++)
    pinMode(i, OUTPUT);
}

void loop() {
  Serial.print("Received Speed: ");
  Serial.print(receivedSpeed);
  Serial.println(" km/h");
  int ten = receivedSpeed/10, one = receivedSpeed%10;
  displayNum(ten, 0);
  displayNum(one, 7);
  delay(500);
}

// Function to receive data from ESP32
void receiveEvent(int bytes) {
  if (Wire.available()) {
    receivedSpeed = Wire.read();  // Read the integer speed value
  }
}

void displayNum(int n, int x) {
  switch(n) {
  case 0://0
  clear(x);
  for(int i = 8-x; i<=13-x; i++)
  digitalWrite(i, LOW);
  break;
  case 1://1
  clear(x);
  digitalWrite(b-x, LOW);
  digitalWrite(c-x, LOW);
  break;
  case 2://2
  clear(x);
  digitalWrite(a-x, LOW);
  digitalWrite(b-x, LOW);
  digitalWrite(d-x, LOW);
  digitalWrite(e-x, LOW);
  digitalWrite(g-x, LOW);
  break;
  case 3://3
  clear(x);
  for(int i = 13-x; i>=10-x; i--)
    digitalWrite(i, LOW);
  digitalWrite(g-x, LOW);
  break;
  case 4://4
  clear(x);
  digitalWrite(b-x, LOW);
  digitalWrite(c-x, LOW);
  digitalWrite(f-x, LOW);
  digitalWrite(g-x, LOW);
  break;
  case 5://5
  clear(x);
  digitalWrite(a-x, LOW);
  digitalWrite(f-x, LOW);
  digitalWrite(g-x, LOW);
  digitalWrite(c-x, LOW);
  digitalWrite(d-x, LOW);
  break;
  case 6://6
  clear(x);
  for(int i = 7-x; i<=13-x; i++)
    if(i!=12-x)
      digitalWrite(i, LOW);
  break;
  case 7://7
  clear(x);
  for(int i = 11; i<=13; i++)
    digitalWrite(i-x, LOW);
  break;
  case 8://8
  clear(x);
  for(int i = 7; i<=13; i++)
    digitalWrite(i-x, LOW);
  break;
  case 9://9
  clear(x);
  for(int i = 13-x; i>=7-x; i--)
    if(i!=9-x)
      digitalWrite(i, LOW);
  break;
}
}

void clear(int x) {
  for(int j = 7-x; j<=13-x; j++)
    digitalWrite(j, HIGH);
}