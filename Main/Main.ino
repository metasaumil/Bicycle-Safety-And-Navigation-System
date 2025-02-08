#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <Adafruit_GFX.h>
#include <freertos/task.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <freertos/FreeRTOS.h>

#define left 12
#define right 14
#define brake 26
#define SS_PIN 5          
#define buzzer 27
#define RST_PIN 22        
#define SERVO_PIN 13      
#define SCREEN_WIDTH 128  
#define SCREEN_HEIGHT 64  

Servo myServo;
MFRC522 mfrc522(SS_PIN, RST_PIN);

volatile bool isLocked = true;
String authorizedUID = "86 3F 27 25";

Adafruit_MPU6050 mpu;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int lastv = 0, v = 0;
unsigned long lockTime = 0;
unsigned long speedZeroTime = 0;
float t = 0, t2 = 0, interval = 0;
unsigned long lastDisplayUpdate = 0;

TaskHandle_t Task1;
TaskHandle_t Task2;

//oled + mpu
void task1(void *pvParameters) {
  volatile bool *isLocked = (volatile bool *) pvParameters;
  while (1) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    t = millis();
    interval = (t - t2) / 1000.0;
    float x = g.gyro.x;
    lastv = v;
    v += (x * interval * 3.6);
    if(lastv>v&&v!=0)
      digitalWrite(brake, HIGH);
    else 
      digitalWrite(brake, LOW);
    if (v < 0) v = 0;
    
    if(!(*isLocked)) {
      if(a.acceleration.y<-4) {
        digitalWrite(right, LOW);
        digitalWrite(left, HIGH);
        digitalWrite(buzzer, HIGH);
        delay(300);
        digitalWrite(buzzer, LOW);
        digitalWrite(left, LOW);
      } else if(a.acceleration.y<-2) {
        digitalWrite(right, LOW);
        digitalWrite(left, HIGH);
        digitalWrite(buzzer, HIGH);
        delay(700);
        digitalWrite(buzzer, LOW);
        digitalWrite(left, LOW);
      } else if(a.acceleration.y>4) {
        digitalWrite(left, LOW);
        digitalWrite(right, HIGH);
        digitalWrite(buzzer, HIGH);
        delay(300);
        digitalWrite(buzzer, LOW);
        digitalWrite(right, LOW);
      } else if(a.acceleration.y>2) {
        digitalWrite(left, LOW);
        digitalWrite(right, HIGH);
        digitalWrite(buzzer, HIGH);
        delay(700);
        digitalWrite(buzzer, LOW);
        digitalWrite(right, LOW);
      } else {
        digitalWrite(right, LOW);
        digitalWrite(left, LOW);
      }
    }

    if (v == 0) {
      if (speedZeroTime == 0) {
        speedZeroTime = millis();
      } else if (millis() - speedZeroTime > 5000) {
        if (*isLocked && millis() - lockTime > 5000) {
          display.clearDisplay();
          display.display();
        } else if (!(*isLocked)) {
          homeScreen();
        }
        continue;
      }
    } else speedZeroTime = 0;
  
    t2 = t;
    if (!(*isLocked))
      displaySpeed(v);

    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}

//RFID
void task2(void *pvParameters) {
  volatile bool *isLocked = (volatile bool *) pvParameters;
  while (1) {
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
        *isLocked = !(*isLocked);
        lockTime = millis();
        if (*isLocked) {
          Serial.println("Locking...");
          myServo.write(0);
          digitalWrite(buzzer, HIGH);
          delay(200);
          digitalWrite(buzzer, LOW);
        } else {
          Serial.println("Unlocking...");
          myServo.write(90);
          digitalWrite(buzzer, HIGH);
          delay(200);
          digitalWrite(buzzer, LOW);
        }
        digitalWrite(right, HIGH);
        digitalWrite(left, HIGH);
        digitalWrite(brake, HIGH);
        delay(1000);
        digitalWrite(right, LOW);
        digitalWrite(left, LOW);
        digitalWrite(brake, LOW);
        displayLock();
      } else {
        Serial.println("Access Denied!");
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);

        String str = "ACCESS";
        int textWidth1 = str.length() * 12;
        int x_center1 = (SCREEN_WIDTH - textWidth1) / 2;
        display.setCursor(x_center1, 5);
        display.print(str);
  
        String secondStr = "DENIED"; 
        int textWidth2 = secondStr.length() * 12;
        int x_center2 = (SCREEN_WIDTH - textWidth2) / 2;
        display.setCursor(x_center2, 30); 
        display.print(secondStr);

        display.display();

        for(int i = 0; i<15; i++) {
          digitalWrite(right, HIGH);
          digitalWrite(left, HIGH);
          digitalWrite(buzzer, HIGH);
          digitalWrite(brake, HIGH);
          delay(200);
          digitalWrite(brake, LOW);
          digitalWrite(right, LOW);
          digitalWrite(left, LOW);
          digitalWrite(buzzer, LOW);
          delay(200);
        }
      }

      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
    }
    
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}

void setup() {
  pinMode(left, OUTPUT);
  pinMode(brake, OUTPUT);
  pinMode(right, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);
  while (!Serial) delay(10);

  Wire.begin(21, 22);

  Serial.println("Initializing...");
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) delay(10);
  } else Serial.println("MPU6050 initialized!");

  SPI.begin();
  mfrc522.PCD_Init();

  myServo.attach(SERVO_PIN);
  myServo.write(0);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 initialization failed"));
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  String secondStr = "WELCOME!"; 
  int textWidth2 = secondStr.length() * 12;
  int x_center2 = (SCREEN_WIDTH - textWidth2) / 2;
  display.setCursor(x_center2, 5); 
  display.print(secondStr);
  display.display();
  delay(3000);

  homeScreen();

  xTaskCreatePinnedToCore(task1, "Task1", 10000, (void *)&isLocked, 1, &Task1, 1);
  xTaskCreatePinnedToCore(task2, "Task2", 4096, (void *)&isLocked, 1, &Task2, 0);
}

void loop() {}

void homeScreen() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  String str = isLocked ? "LOCKED" : "UNLOCKED";
  int textWidth1 = str.length() * 12;
  int x_center1 = (SCREEN_WIDTH - textWidth1) / 2;
  display.setCursor(x_center1, 5);
  display.print(str);
  
  display.setTextSize(2);
  String secondStr = "Batt: XX%"; 
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
