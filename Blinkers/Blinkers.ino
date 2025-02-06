#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#define right 23
#define left 15
//scl to 22
//sdl to 21

Adafruit_MPU6050 mpu;

void setup() {
  pinMode(right, OUTPUT);
  pinMode(left, OUTPUT);
  Serial.begin(115200);
  while (!Serial)
    delay(10);
  Serial.println("Adafruit MPU6050 test!");
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  Serial.println("");
  delay(100);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");
  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");
  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");
  Serial.println("");
  if(a.acceleration.y<-1) {
    digitalWrite(right, LOW);
    digitalWrite(left, HIGH);
    delay(700);
    digitalWrite(left, LOW);
    // delay(500);
  } else if(a.acceleration.y>1) {
    digitalWrite(left, LOW);
    digitalWrite(right, HIGH);
    delay(700);
    digitalWrite(right, LOW);
    // delay(500);
  } else {
    digitalWrite(right, LOW);
    digitalWrite(left, LOW);
  }
  delay(500);
}