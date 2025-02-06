//gps+blinkers+velocity
// MPU6050: SDA to 21, SCL to 22
// NEO6M: TX to 16 (RX2), RX to 17 (TX2)

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <TinyGPS++.h>

#define I2C_SLAVE_ADDR 8  
#define right 15
#define left 23

float t = 0, t2 = 0, interval = 0, v = 0;

Adafruit_MPU6050 mpu;
HardwareSerial neogps(1);  // Use UART1 on ESP32
TinyGPSPlus gps;

void setup() {
  pinMode(right, OUTPUT);
  pinMode(left, OUTPUT);
  
  Wire.begin();
  Serial.begin(115200);
  neogps.begin(9600, SERIAL_8N1, 16, 17); // GPS on UART1

  while (!Serial) delay(10);

  Serial.println("Initializing MPU6050...");
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) delay(10);
  }
  Serial.println("MPU6050 Found!");

  Serial.println("GPS initialized.");
  delay(2000);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  Serial.print("Acceleration X: "); Serial.print(a.acceleration.x);
  Serial.print(", Y: "); Serial.print(a.acceleration.y);
  Serial.print(", Z: "); Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: "); Serial.print(g.gyro.x);
  Serial.print(", Y: "); Serial.print(g.gyro.y);
  Serial.print(", Z: "); Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: "); Serial.print(temp.temperature);
  Serial.println(" degC\n");

  // Read GPS data
  boolean newData = false;
  unsigned long start = millis();
  
  while (millis() - start < 1000) {
    while (neogps.available()) {
      if (gps.encode(neogps.read())) {
        newData = true;
      }
    }
  }

  if (newData) {
    Serial.println("GPS Data Received:");
    print_gps();
  } else {
    Serial.println("No GPS data.");
  }

  // Calculate velocity
  t = millis();
  interval = (t - t2) / 1000.0;
  v += g.gyro.y * interval * 3.6;
  if (v < 0) v = 0;
  t2 = t;
  sendSpeedToSlave(v);

  // Motor control based on acceleration
  if(a.acceleration.y <-4) {
    digitalWrite(right, LOW);
    digitalWrite(left, HIGH);
    delay(200);
    digitalWrite(left, LOW);
  } else if (a.acceleration.y < -1) {
    digitalWrite(right, LOW);
    digitalWrite(left, HIGH);
    delay(400);
    digitalWrite(left, LOW);
  } else if (a.acceleration.y > 4) {
    digitalWrite(left, LOW);
    digitalWrite(right, HIGH);
    delay(200);
    digitalWrite(right, LOW);
  } else if (a.acceleration.y > 1) {
    digitalWrite(left, LOW);
    digitalWrite(right, HIGH);
    delay(400);
    digitalWrite(right, LOW);
  } else {
    digitalWrite(right, LOW);
    digitalWrite(left, LOW);
  }
}

void print_gps() {
  if (gps.location.isValid()) {
    Serial.print("Latitude: "); Serial.print(gps.location.lat(), 6);
    Serial.print(", Longitude: "); Serial.println(gps.location.lng(), 6);
    Serial.print("Speed: "); Serial.println(gps.speed.kmph());
    Serial.print("Satellites: "); Serial.println(gps.satellites.value());
    Serial.print("Altitude: "); Serial.println(gps.altitude.meters());
  } else {
    Serial.println("GPS signal lost.");
  }
}

void sendSpeedToSlave(float speed) {
  Wire.beginTransmission(I2C_SLAVE_ADDR);
  Wire.write((int)speed); 
  Wire.endTransmission();
}
