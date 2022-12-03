#include <SPI.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

Servo ServoUpper;
Servo ServoLower;

const int ServoUpperPin = 6;
const int ServoLowerPin = 5;
int ServoUpperValue = 0;
int ServoLowerValue = 0;
int ServoUpperMaxValue = 0;
int ServoUpperMinValue = 0;

const int MCP3008_cs = 10;
const int ModeButton = 8;


//Pins on the ADC
const int LCR0Pin = 0;
const int LCR1Pin = 1;
const int LCR2Pin = 2;
const int LCR3Pin = 3;

//Pins on the ADC
const int VsolarPin = 4;
const int PotiPin = 5;

bool prevButton = false;
bool currButton = false;

int Mode = 0;

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
  ServoLower.attach(ServoLowerPin);
  ServoUpper.attach(ServoUpperPin);
  
  pinMode(MCP3008_cs, OUTPUT);
  pinMode(ModeButton, INPUT);
  digitalWrite(MCP3008_cs, HIGH);
  lcd.createChar(0, ModeOff);
  lcd.createChar(1, ModeAuto);
  lcd.createChar(2, ModeLower);
  lcd.createChar(3, ModeUpper);
  
}

void loop() {
  //Mode 0 = Off
  //Mode 1 = Auto
  //Mode 2 = Manual Lower
  //Mode 3 = Manual Upper
  currButton = digitalRead(ModeButton);
  if(currButton == true && prevButton == false)
  {
    Mode++;

    if(Mode > 3)
    {
      Mode = 0;
    }
  }
  prevButton = currButton;
  WriteModeToLCD();

  switch(Mode)
  {
    //Mode Off
    case 0:
      ModeOff();
      break;
      
    //Mode Auto
    case 1:
      ModeAuto();
      break;

    //Mode Manual Lower
    case 2:
      ModeManLower();
      break;

    //Mode Manual Upper
    case 3:
      ModeManUpper();
      break;
  }
  
}

void ModeOff()
{
  
}

void ModeAuto()
{
  
}

void ModeManLower()
{
  
}

void ModeManUpper()
{
  
}

void WriteInfoToLCD()
{
  
}

//Read analog value from ADC (MCP3008)
uint16_t mcp3008_read(uint8_t channel) {
  digitalWrite(CS, LOW);
  SPI.transfer(0x01);
  uint8_t msb = SPI.transfer(0x80 + (channel << 4));
  uint8_t lsb = SPI.transfer(0x00);
  digitalWrite(CS, HIGH);
  return ((msb & 0x03) << 8) + lsb;
}


//Method to write the current Operation Mode to the LCD
void WriteModeToLCD()
{
  lcd.setCursor(0, 0);
  lcd.print("Mode: ");
  switch(Mode)
  {
    case 0:
      lcd.write(0);
      break;
    case 1:
      lcd.write(1);
      break;
    case 2:
      lcd.write(2);
      break;
    case 3:
      lcd.write(3);
      break;
  }
}