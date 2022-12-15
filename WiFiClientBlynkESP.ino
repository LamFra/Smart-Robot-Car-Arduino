#define BLYNK_TEMPLATE_ID "" //put the blynk template id
#define BLYNK_DEVICE_NAME "" //put the blynk device name 
#define BLYNK_AUTH_TOKEN "" //put the blynk auth token

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#ifndef STASSID
#define STASSID "" //put the wifi's name
#define STAPSK  "" //put the wifi's password
#endif

#define BLYNK_PRINT Serial

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels


char auth[] = BLYNK_AUTH_TOKEN;

const char* ssid = STASSID;
const char* pass = STAPSK;
const int MPU=0x68; 

BlynkTimer timer;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void updateAccelerometerAndGyroscope()
{
  int AX, AY, AZ, GX, GY, GZ, Tmp;
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true); // request a total of 14 registers
  AX=Wire.read()<<8|Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AY=Wire.read()<<8|Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AZ=Wire.read()<<8|Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GX=Wire.read()<<8|Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GY=Wire.read()<<8|Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GZ=Wire.read()<<8|Wire.read();
  Blynk.virtualWrite(V0, AX);
  Blynk.virtualWrite(V1, AY);
  Blynk.virtualWrite(V2, AZ);
  Blynk.virtualWrite(V3, GX);
  Blynk.virtualWrite(V4, GY);
  Blynk.virtualWrite(V5, GZ);
  String messageBlynk = "AX: " + String(AX) + " AY: " + String(AY) + " AZ: " + String(AZ) + "\n" + " GX: " + String(GX) + " GY: " + String(GY) + " GZ: " + String(GZ) + "\n\n";
  String messageOled = "Axel: " + String(AX) +" " + String(AY) + " " + String(AZ) + "\n\nGyro: " + String(GX) + " " + String(GY) + " " + String(GZ); 
  Blynk.virtualWrite(V6, messageBlynk);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(messageOled);
  display.display();
  Serial.println(messageOled);
}

BLYNK_CONNECTED()
{
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
  }
  else{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("Connected to wifi:");
    display.println(ssid);
    display.display();
    delay(3000);
  }
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(3000L, updateAccelerometerAndGyroscope); //Staring a timer
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}

void loop() {
  Blynk.run();
  timer.run();
}
