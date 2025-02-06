#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
//scl to 22
//sdl to 21

float t = 0, t2 = 0, interval = 0, v = 0;

Adafruit_MPU6050 mpu;

void setup() {
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
  t = millis();
  interval = (t - t2)/1000.0;
  float x = g.gyro.x;
  v += (x * interval * 3.6);
  if(v<0) v = 0;
  Serial.print("Acceleration = ");
  Serial.print(x);
  Serial.println(" m/s^2");
  Serial.print("Speed = ");
  Serial.print(v);
  Serial.println(" km/h");
  t2 = t;
  delay(1000);
}
