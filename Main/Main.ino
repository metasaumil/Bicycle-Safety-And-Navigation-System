#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>

#define SS_PIN 5          
#define RST_PIN 4
#define SERVO_PIN 13      
#define SCREEN_WIDTH 128  
#define SCREEN_HEIGHT 64  

Servo myServo;
MFRC522 mfrc522(SS_PIN, RST_PIN);

String authorizedUID = "86 3F 27 25";
bool isLocked = true;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_MPU6050 mpu;

int v = 0;
unsigned long speedZeroTime = 0;
float t = 0, t2 = 0, interval = 0;

void setup() {

  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("Start");

  Serial.println("Initializing...");
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) delay(10);
  }
  
  Wire.begin(21, 22);
  SPI.begin();
  mfrc522.PCD_Init();
  
  myServo.attach(SERVO_PIN);
  myServo.write(0);
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 initialization failed"));
    for (;;);
  }
  
  display.clearDisplay();
  display.display();
  homeScreen();
}

void loop() {

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  t = millis();
  interval = (t - t2) / 1000.0;
  float x = g.gyro.x;
  v += (x * interval * 3.6);
  if (v < 0) v = 0;

  // Serial.print("Speed = ");
  // Serial.print(v);
  // Serial.println(" km/h");
  
  if (v == 0) {
    if (speedZeroTime == 0) {
      speedZeroTime = millis();
    } else if (millis() - speedZeroTime > 5000) {
      homeScreen();
      return;
    }
  } else {
    speedZeroTime = 0;
  }
  
  t2 = t;
  // delay(1000);
  displaySpeed(v);

  if(v==0) {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      String scannedUID = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        scannedUID += String(mfrc522.uid.uidByte[i], HEX);
        if (i < mfrc522.uid.size - 1) scannedUID += " ";
      }
      scannedUID.toUpperCase();
      Serial.print("Scanned UID: ");
      Serial.println(scannedUID);

      if (scannedUID == authorizedUID) {
        isLocked = !isLocked; 
        if (isLocked) {
          Serial.println("Locking...");
          myServo.write(0);
        } else {
          Serial.println("Unlocking...");
          myServo.write(90);
        }
        displayLock();
      } else {
        Serial.println("Access Denied!");
      }
    
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
    }
  }
}

void homeScreen() {

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  String str = isLocked ? "LOCKED" : "UNLOCKED";
  int textWidth1 = str.length() * 12;
  int x_center1 = (SCREEN_WIDTH - textWidth1) / 2;
  display.setCursor(x_center1, 5);
  display.print(str);

  String secondStr = "Something"; 
  int textWidth2 = secondStr.length() * 12;
  int x_center2 = (SCREEN_WIDTH - textWidth2) / 2;
  display.setCursor(x_center2, 30); 
  display.print(secondStr);

  display.display();
}


void displayLock() {

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  String str = isLocked ? "LOCKED" : "UNLOCKED";
  int textWidth = str.length() * 12; 
  int x_center = (SCREEN_WIDTH - textWidth) / 2;
  display.setCursor(x_center, 5); 
  display.print(str);
  display.display();
}

void displaySpeed(int num) {

  display.clearDisplay();
  display.setTextSize(9);
  display.setTextColor(SSD1306_WHITE);

  String numStr = String(num);
  int textWidth = numStr.length() * 6 * 9;
  int textHeight = 8 * 9;
  int x_center = (SCREEN_WIDTH - textWidth) / 2;
  int y_center = (SCREEN_HEIGHT - textHeight) / 2;

  display.setCursor(x_center, y_center);
  display.print(numStr);
  display.display();
}
