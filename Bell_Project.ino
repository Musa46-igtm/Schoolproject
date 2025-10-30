#include <Wire.h>
#include "RTClib.h"
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

RTC_DS1307 rtc;
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

struct BellTime {
  uint8_t hour;
  uint8_t minute;
  uint8_t dayOfWeek;
  bool hasRung;
};

BellTime bells[] = {
  {7,45,1,false}, {8,0,1,false}, {8,40,1,false}, {9,20,1,false},
  {10,0,1,false}, {10,40,1,false}, {11,20,1,false}, {12,0,1,false},
  {12,40,1,false}, {13,20,1,false}, {14,0,1,false},
  
  {7,45,2,false}, {8,0,2,false}, {8,40,2,false}, {9,20,2,false},
  {10,0,2,false}, {10,40,2,false}, {11,20,2,false}, {12,0,2,false},
  {12,40,2,false}, {13,20,2,false}, {14,0,2,false},

  {7,45,3,false}, {8,0,3,false}, {8,40,3,false}, {9,20,3,false},
  {10,0,3,false}, {10,40,3,false}, {11,20,3,false}, {12,0,3,false},
  {12,40,3,false}, {13,0,3,false}, {14,0,3,false},

  {7,45,4,false}, {8,0,4,false}, {8,40,4,false}, {9,20,4,false},
  {10,0,4,false}, {10,40,4,false}, {11,20,4,false}, {12,0,4,false},
  {12,40,4,false}, {13,20,4,false}, {14,0,4,false},

  {7,45,5,false}, {8,0,5,false}, {8,40,5,false}, {9,20,5,false},
  {10,0,5,false}, {10,40,5,false}, {11,20,5,false}, {12,0,5,false},
  {12,40,5,false}, {13,0,5,false}
};

const int numBells = sizeof(bells) / sizeof(bells[0]);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mySoftwareSerial.begin(9600);

  delay(1000);

  if (!rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println(F("RTC not running, setting time now..."));
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Couldn't find DFPlayer"));
    while (1);
  }

  myDFPlayer.volume(25);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);

  Serial.println(F("System ready."));
}

void loop() {
  DateTime now = rtc.now();
  static uint8_t lastMinute = 255;

  if (now.minute() != lastMinute) {
    lastMinute = now.minute();

    for (int i = 0; i < numBells; i++) {
      if (bells[i].dayOfWeek == now.dayOfTheWeek() &&
          bells[i].hour == now.hour() &&
          bells[i].minute == now.minute() &&
          !bells[i].hasRung) {
        
        Serial.print(F("🔔 Bell ringing at "));
        Serial.print(now.hour());
        Serial.print(":");
        Serial.println(now.minute());
        myDFPlayer.play(1); // plays 0001.mp3
        bells[i].hasRung = true;
        delay(5000);
      } else {
        bells[i].hasRung = false;
      }
    }
  }

  delay(500);
}