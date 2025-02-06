#include <TinyGPS++.h>

HardwareSerial neogps(1);
TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);
  neogps.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("GPS found!");
  delay(2000);
}

void loop() {
  boolean newData = false;
  for (unsigned long start = millis(); millis()-start<1000;)
    while (neogps.available())
      if (gps.encode(neogps.read()))
        newData = true;
  if(newData == true) {
    newData = false;
    Serial.println(gps.satellites.value());
    print_speed();
  } else
    Serial.println("No data");
  delay(1000);
}

void print_speed() {
  if (gps.location.isValid() == 1) {
    Serial.print("Latitude: ");
    Serial.print(gps.location.lat());
    Serial.print(", ");
    Serial.println(gps.location.lng());
    Serial.print("Speed: ");
    Serial.println(gps.speed.kmph());
    Serial.print("Satellatie value: ");
    Serial.println(gps.satellites.value());
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
  } else
    Serial.println("No data");
}