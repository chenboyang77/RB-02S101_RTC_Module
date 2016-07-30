#include <TimerOne.h>
#include "TM1637.h"
#include <Wire.h>
#include "RTClib.h"
#define ON 1
#define OFF 0
#if defined(ARDUINO_ARCH_SAMD)
#define Serial SerialUSB
#endif

RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};
unsigned char ClockPoint = 1;
unsigned char Update;
unsigned char minute = 0;
unsigned char hour = 12;
int stem,val;

#define CLK 5
#define DIO 4
TM1637 tm1637(CLK,DIO);

void setup()
{
  tm1637.set();
  tm1637.init();
  Timer1.initialize(500000);
  Timer1.attachInterrupt(TimingISR);
  #ifndef ESP8266
  while (!Serial); 
  #endif
  Serial.begin(57600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
     rtc.adjust(DateTime(2016, 7, 27, 13, 0, 0));
  }
}
void loop()
{
  DateTime now = rtc.now();  
  if(Update == ON)
  {  
    TimeUpdate();
    tm1637.display(TimeDisp);
  }
    stem = now.hour();
    val = now.minute();
}

void TimingISR()
{
    ClockPoint = (~ClockPoint) & 0x01;
    Update = ON;
}
void TimeUpdate(void)
{
  if(ClockPoint)tm1637.point(POINT_ON);
  else tm1637.point(POINT_OFF);
  TimeDisp[0] = stem/ 10;
  TimeDisp[1] = stem % 10;
  TimeDisp[2] = val / 10;
  TimeDisp[3] = val % 10;
}
