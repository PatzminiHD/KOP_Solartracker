#include <SPI.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

Servo ServoUpper;
Servo ServoLower;

const int ServoUpperPin = 6;
const int ServoLowerPin = 5;
int ServoUpperValue = 0;
int ServoLowerValue = 0;
const int ServoUpperMaxValue = 180;
const int ServoUpperMinValue = 30;
const int ServoLowerMaxValue = 180;
const int ServoLowerMinValue = 0;

const int MCP3008_cs = 10;
const int ModeButton = 8;

//Pins on the ADC
const int LCR0Pin = 0;
const int LCR1Pin = 1;
const int LCR2Pin = 2;
const int LCR3Pin = 3;

const int ThresholdValue = 20;
const int ServoSteps = 1;

//Pins on the ADC
const int VsolarPin = 4;
const int PotiPin = 5;

bool prevButton = false;
bool currButton = false;

int Mode = 0;

byte ModeOffChar[] = {
  B01110,
  B10001,
  B10001,
  B10101,
  B10101,
  B10001,
  B10001,
  B01110
};
byte ModeLowerChar[] = {
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
  B00000,
  B00000
};
byte ModeUpperChar[] = {
  B00000,
  B00000,
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100
};
byte ModeAutoChar[] = {
  B00000,
  B01110,
  B10001,
  B10001,
  B10001,
  B11111,
  B10001,
  B10001
};

byte degree[] = {
  B01100,
  B10010,
  B10010,
  B01100,
  B00000,
  B00000,
  B00000,
  B00000
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
  lcd.createChar(0, ModeOffChar);
  lcd.createChar(1, ModeAutoChar);
  lcd.createChar(2, ModeLowerChar);
  lcd.createChar(3, ModeUpperChar);
  lcd.createChar(4, degree);

}

void loop() {
  //Mode 0 = Off
  //Mode 1 = Auto
  //Mode 2 = Manual Lower
  //Mode 3 = Manual Upper
  currButton = digitalRead(ModeButton);
  if (currButton == true && prevButton == false)
  {
    Mode++;

    if (Mode > 3)
    {
      Mode = 0;
    }
  }
  prevButton = currButton;

  switch (Mode)
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
  SetServos();
  WriteInfoToLCD();
}

void ModeOff()
{
  ServoLowerValue = 90;
  ServoUpperValue = 90;
}

void ModeAuto()
{
    int LDR0Value = mcp3008_read(LCR0Pin);
    int LDR1Value = mcp3008_read(LCR1Pin);
    int LDR2Value = mcp3008_read(LCR2Pin);
    int LDR3Value = mcp3008_read(LCR3Pin);

    if(LDR0Value - ThresholdValue > LDR2Value)
    {
      ServoUpperValue += ServoSteps;
    }
    else if(LDR2Value - ThresholdValue > LDR0Value)
    {
      ServoUpperValue -= ServoSteps;
    }

    if(LDR1Value - ThresholdValue > LDR3Value)
    {
      ServoLowerValue += ServoSteps;
    }
    else if(LDR3Value - ThresholdValue > LDR1Value)
    {
      ServoUpperValue -= ServoSteps;
    }
}

void ModeManLower()
{
  int PotiValue = mcp3008_read(PotiPin);
  ServoLowerValue = map(PotiValue, 0, 1023, 0, 180);
}

void ModeManUpper()
{
  int PotiValue = mcp3008_read(PotiPin);
  ServoUpperValue = map(PotiValue, 0, 1023, 0, 180);
}

void WriteInfoToLCD()
{
  int VsolarValue = mcp3008_read(VsolarPin);
  VsolarValue = map(VsolarValue, 0, 1023, 0, 5000);

  WriteModeToLCD();
  lcd.setCursor(0, 1);
  lcd.print("SrvLwr: ");
  lcd.print(ServoLowerValue);
  lcd.write(4);
  lcd.print("   ");

  lcd.setCursor(0, 2);
  lcd.print("SrvUpr: ");
  lcd.print(ServoUpperValue);
  lcd.write(4);
  lcd.print("   ");

  lcd.setCursor(0, 3);
  lcd.print("Vsolar= ");
  lcd.print(VsolarValue);
  lcd.print("mV      ");
}

void SetServos()
{
  //Cap Value of Upper Servo
  if (ServoUpperValue > ServoUpperMaxValue)
  {
    ServoUpperValue = ServoUpperMaxValue;
  }
  if (ServoUpperValue < ServoUpperMinValue)
  {
    ServoUpperValue = ServoUpperMinValue;
  }

  //Cap Value of Lower Servo
  if (ServoLowerValue > ServoLowerMaxValue)
  {
    ServoLowerValue = ServoLowerMaxValue;
  }
  if (ServoLowerValue < ServoLowerMinValue)
  {
    ServoLowerValue = ServoLowerMinValue;
  }
  ServoLower.write(ServoLowerValue);
  ServoUpper.write(ServoUpperValue);
}

//Read analog value from ADC (MCP3008)
uint16_t mcp3008_read(uint8_t channel) {
  digitalWrite(MCP3008_cs, LOW);
  SPI.transfer(0x01);
  uint8_t msb = SPI.transfer(0x80 + (channel << 4));
  uint8_t lsb = SPI.transfer(0x00);
  digitalWrite(MCP3008_cs, HIGH);
  return ((msb & 0x03) << 8) + lsb;
}


//Method to write the current Operation Mode to the LCD
void WriteModeToLCD()
{
  lcd.setCursor(0, 0);
  lcd.print("Mode: ");
  switch (Mode)
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
