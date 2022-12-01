#include <Servo.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

Servo servo_upper;
Servo servo_lower;

const int ServoUpperPin = 6;
const int ServoLowerPin = 5;

const int MCP3008_cs = 10;
const int ModeButton = 8;

bool prevSwitch = false;



void setup() {
  
  
}

void loop() {
  
}
