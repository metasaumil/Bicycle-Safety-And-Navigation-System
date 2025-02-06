//esp32

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define right 23
#define left 15

#define I2C_SLAVE_ADDR 8  
float t = 0, t2 = 0, interval = 0, v = 0;
Adafruit_MPU6050 mpu;

void setup() {
  pinMode(right, OUTPUT);
  pinMode(left, OUTPUT);

  Serial.begin(115200);
  Wire.begin();

  while (!Serial) delay(10);
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
  interval = (t - t2) / 1000.0;
  float x = g.gyro.x;
  v += (x * interval * 3.6);
  if (v < 0) v = 0;

  Serial.print("Speed = ");
  Serial.print(v);
  Serial.println(" km/h");

  sendSpeedToSlave(v);

  t2 = t;

  if (a.acceleration.y < -1) {
    digitalWrite(right, LOW);
    digitalWrite(left, HIGH);
    delay(700);
    digitalWrite(left, LOW);
  } else if (a.acceleration.y > 1) {
    digitalWrite(left, LOW);
    digitalWrite(right, HIGH);
    delay(700);
    digitalWrite(right, LOW);
  } else {
    digitalWrite(right, LOW);
    digitalWrite(left, LOW);
  }
  delay(500);
}

  void sendSpeedToSlave(float speed) {
    Wire.beginTransmission(I2C_SLAVE_ADDR);
    Wire.write((int)speed);  // Send integer value of speed
    Wire.endTransmission();
  }
