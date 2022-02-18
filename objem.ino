#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#define VOLUME_EVENT 0
#define DISPLAY_EVENT 1
#define OLED_01_RESET 4
#define OLED_02_CLK 15
#define OLED_02_MOSI 16
#define OLED_02_RESET 6
#define OLED_02_DC 4
#define OLED_02_CS 5
#define FADER_0 A0
#define FADER_1 A1
#define FADER_TOUCH_IN_0 A3
#define FADER_TOUCH_OUT_0 A2
#define FADER_TOUCH_IN_1 A4
#define FADER_TOUCH_OUT_1 A5
#define AIN1 12
#define AIN2 9
#define BIN1 13
#define BIN2 14
#define APWM 10
#define BPWM 11
#define ASTBY 0
#define BSTBY 1
#define SPEED 255
Adafruit_SH1106 display_01(OLED_01_RESET);
Adafruit_SH1106 display_02(OLED_02_MOSI, OLED_02_CLK, OLED_02_DC, OLED_02_RESET, OLED_02_CS);
Adafruit_SH1106 displays[2] = {display_01, display_02};
String display_01_text = "";
String display_02_text = "";
const int NEXT_BTN = 7;
const int PREV_BTN = 8;
bool isReleased = true;

uint8_t fader0Target = 50;
uint8_t fader1Target = 50;

bool isTouchFader0 = false;
bool isTouchFader1 = false;

uint8_t threshold0 = 0;
uint8_t threshold1 = 0;

uint8_t currentValue0 = 255;
uint8_t currentValue1 = 255;

uint8_t prevCounter0 = 0;
uint8_t prevCounter1 = 0;

double f = 0;

uint32_t serialTime;
void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(1);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(APWM, OUTPUT);
  pinMode(ASTBY, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(BPWM, OUTPUT);
  pinMode(BSTBY, OUTPUT);
  pinMode(FADER_TOUCH_OUT_0, OUTPUT);
  pinMode(FADER_TOUCH_IN_0, INPUT);
  pinMode(FADER_TOUCH_OUT_1, OUTPUT);
  pinMode(FADER_TOUCH_IN_1, INPUT);
  calibrateTouchSensor();
  //ディスプレイ初期化
  displays[0].begin(SH1106_SWITCHCAPVCC, 0x3C);
  displays[1].begin(SH1106_SWITCHCAPVCC);
  for (uint8_t i = 0; i < 2; i++)
  {
    displays[i].clearDisplay();
    displays[i].setTextSize(2);
    displays[i].setTextColor(WHITE);
    displays[i].display();
  }
  display(0, "Objem");
  display(1, "Ready...");
  pinMode(NEXT_BTN, INPUT_PULLUP);
  pinMode(PREV_BTN, INPUT_PULLUP);
}
void loop()
{
  uint8_t A = map(analogRead(FADER_0), 0, 1023, 0, 100);
  if (isTouchFader0) {
    standbyA();
    if (currentValue0 != A) {
      currentValue0 = A;
      Serial.println("0:" + (String)currentValue0);
    }
  } else {
    if (abs( A - fader0Target ) > 3) {
      if (A > fader0Target) {
        downA();
      } else if (A < fader0Target) {
        upA();
      }
    } else {
      standbyA();
    }
  }

  uint8_t B = map(analogRead(FADER_1), 0, 1023, 0, 100);
  if (isTouchFader1) {
    standbyB();
    if (currentValue1 != B) {
      currentValue1 = B;
      Serial.println("1:" + (String)B);
    }
  } else {
    if (abs( B - fader1Target ) > 3) {
      if (B > fader1Target) {
        downB();
      } else if (B < fader1Target) {
        upB();
      }
    } else {
      standbyB();
    }
  }


  handleEvent();
  ButtonHandler();
  uint8_t temp = 0;
  uint8_t counter = 0;
  digitalWrite(FADER_TOUCH_OUT_0, HIGH);
  while (digitalRead(FADER_TOUCH_IN_0) == LOW && counter <= threshold0) {
    counter++;
  }
  if(counter==0)counter =  prevCounter0;
  prevCounter0 = counter;
  //Serial.println(counter);
  isTouchFader0 = counter  > threshold0;
  counter = 0;
  digitalWrite(FADER_TOUCH_OUT_1, HIGH);
  while (digitalRead(FADER_TOUCH_IN_1) == LOW && counter <= threshold1) {
    counter++;
  }
  if(counter==0)counter =  prevCounter1;
  prevCounter1 = counter;

  digitalWrite(FADER_TOUCH_OUT_0, LOW);
  digitalWrite(FADER_TOUCH_OUT_1, LOW);

  isTouchFader1 = counter > threshold1;

  if (isTouchFader0) {
  }

  if (isTouchFader1) {
    standbyB();
  }
}
void display(uint8_t num, String str)
{
  displays[num].clearDisplay();
  displays[num].setTextSize(2);
  displays[num].setCursor(8, 10);
  displays[num].println(str);
  displays[num].display();
}
void handleEvent() {
  if (Serial.available())
  {

    String inputEvent = Serial.readStringUntil(':');
    inputEvent.trim();
    if (inputEvent.length() < 1)
    {
      //Serial.println("Error: Event is empty");
      return;
    }


    int event = inputEvent.toInt();
    String inputFader = Serial.readStringUntil(':');
    inputFader.trim();
    if (inputFader.length() < 1)
    {
      //Serial.println("Error: fader is empty");
      return;
    }
    int fader = inputFader.toInt();
    String value = Serial.readStringUntil(':');
    if (value.length() < 1)
    {
      //Serial.println("Error: Value is empty");
      return;
    }
    switch (event)
    {

      case VOLUME_EVENT:
        if (fader == 0) {
          fader0Target = value.toInt();
        } else {
          fader1Target = value.toInt();
        }
        break;
      case DISPLAY_EVENT:
        display(fader, value);
        break;
    }
  }
}
void ButtonHandler() {
  bool isPressNext = digitalRead(NEXT_BTN) == LOW;
  bool isPressPrev = digitalRead(PREV_BTN) == LOW;
  if (isPressNext) {
    if (!isReleased)return;
    Serial.println("next");
    isReleased = false;
  }
  if (isPressPrev) {
    if (!isReleased)return;
    Serial.println("prev");
    isReleased = false;
  }
  if (!isPressNext && !isPressPrev && !isReleased) {
    isReleased = true;
  }
}
void calibrateTouchSensor() {
  uint8_t counter = 0;
  digitalWrite(FADER_TOUCH_OUT_0, LOW);
  digitalWrite(FADER_TOUCH_OUT_0, HIGH);
  while (digitalRead(FADER_TOUCH_IN_0) == LOW) {
    counter++;
  }
  threshold0 = counter + 1;

  counter = 0;
  digitalWrite(FADER_TOUCH_OUT_1, LOW);
  digitalWrite(FADER_TOUCH_OUT_1, HIGH);
  while (digitalRead(FADER_TOUCH_IN_1) == LOW) {
    counter++;
  }
  threshold1 = counter + 1;
}
void upA() {
  digitalWrite(ASTBY, HIGH);
  analogWrite(APWM, SPEED);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
}
void downA() {
  digitalWrite(ASTBY, HIGH);
  analogWrite(APWM, SPEED);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
}
void standbyA() {
  digitalWrite(ASTBY, LOW);
}
void upB() {
  digitalWrite(BSTBY, HIGH);
  analogWrite(BPWM, SPEED);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}
void downB() {
  digitalWrite(BSTBY, HIGH);
  analogWrite(BPWM, SPEED);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
}
void standbyB() {
  digitalWrite(BSTBY, LOW);
}
