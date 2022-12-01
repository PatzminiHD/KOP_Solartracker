#include <Servo.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

Servo ServoUpper;
Servo ServoLower;

const int ServoUpperPin = 6;
const int ServoLowerPin = 5;

const int MCP3008_cs = 10;
const int ModeButton = 8;

bool prevSwitch = false;

byte ModeOff[] = {
  B01110,
  B10001,
  B10001,
  B10101,
  B10101,
  B10001,
  B10001,
  B01110
};
byte ModeLower[] = {
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
  B00000,
  B00000
};
byte ModeUpper[] = {
  B00000,
  B00000,
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100
};
byte ModeAuto[] = {
  B00000,
  B01110,
  B10001,
  B10001,
  B10001,
  B11111,
  B10001,
  B10001
};

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  ServerLower.attach(ServoLowerPin);
  ServerUpper.attach(ServoUpperPin);
  
  pinMode(MCP3008_cs, OUTPUT);
  pinMode(ModeButton, INPUT);
  digitalWrite(MCP3008_cs, HIGH);
  lcd.createChar(0, ModeOff);
  lcd.createChar(1, ModeAuto);
  lcd.createChar(2, ModeLower);
  lcd.createChar(3, ModeUpper);
  
}

void loop() {
  
}
