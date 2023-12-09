/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  ESP32, Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build mobile and web interfaces for any
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: https://www.blynk.io
    Sketch generator:           https://examples.blynk.cc
    Blynk community:            https://community.blynk.cc
    Follow us:                  https://www.fb.com/blynkapp
                                https://twitter.com/blynk_app

  Blynk library is licensed under MIT license
 *************************************************************
  Blynk.Edgent implements:
  - Blynk.Inject - Dynamic WiFi credentials provisioning
  - Blynk.Air    - Over The Air firmware updates
  - Device state indication using a physical LED
  - Credentials reset using a physical Button
 *************************************************************/

/* Fill in information from your Blynk Template here */
/* Read more: https://bit.ly/BlynkInject */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define SS_PIN D8
#define RST_PIN D0


int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
Servo myServo;

MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class

MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[4];

int cambien = D3;
int giatri;
int giatri1;

#define BLYNK_TEMPLATE_ID "TMPL6e3BQ060I"
#define BLYNK_TEMPLATE_NAME "Park"
#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
//#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"
unsigned long times = millis();
WidgetLED LEDCONNECT(V0);
WidgetLED LED1(V1);
WidgetLED LED2(V2);


void setup()
{
  Serial.begin(115200);
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522
  myServo.attach(D4);
  myServo.write(150);  //servo pin

  // initialize LCD
  lcd.init();
  // turn on LCD backlight
  lcd.backlight();
  delay(100);

  BlynkEdgent.begin();
}

void loop() {
  BlynkEdgent.run();
  if (millis() - times > 1000) {
    Blynk.virtualWrite(V5, millis() / 1000);
    if (LEDCONNECT.getValue()) {
      LEDCONNECT.off();
    } else {
      LEDCONNECT.on();
    }
    times = millis();
  }
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Scan Card");
  giatri = digitalRead(cambien);
  Serial.println(giatri);
  Serial.print(" ");

  if (giatri == 1) {
    LED1.off();
  } else {
    LED1.on();
  }
    if (giatri1 == 1) {
    LED2.off();
  } else {
    LED2.on();
  }

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if (!rfid.PICC_ReadCardSerial())
    return;
  Serial.println();
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
    content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(rfid.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "F3 A2 E9 FD")  //nhập mã số thẻ của bạn ở đây
  {
    lcd.setCursor(0, 0);
    lcd.print("Authorized access");
    Serial.println("Authorized access");
    myServo.write(0);
    delay(5000);
    myServo.write(150);  //servo pin
  }
  else {
    lcd.setCursor(0, 0);
    lcd.print("Access is denied");
    Serial.println(" Access is denied");
    delay(5000);
  }
}

