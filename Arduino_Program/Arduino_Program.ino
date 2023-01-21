#include <SPI.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

//Initialise Servos
Servo ServoUpper;
Servo ServoLower;

//Servo Pins on Arduino
const int ServoUpperPin = 6;
const int ServoLowerPin = 5;

//Global Servo Values
int ServoUpperValue = 0;
int ServoLowerValue = 0;

//Global Mode Value
//Mode 0 = Off
//Mode 1 = Auto
//Mode 2 = Manual Lower
//Mode 3 = Manual Upper
int Mode = 0;

//Maximum and Minimum Values for the Servos
const int ServoUpperMaxValue = 160;
const int ServoUpperMinValue = 30;
const int ServoLowerMaxValue = 180;
const int ServoLowerMinValue = 0;

//Chip Select Pin for the ADC
const int MCP3008_cs = 10;

//Mode Button Pin on Arduino
const int ModeButton = 8;

//Solar Voltage Pin on the ADC
const int VsolarPin = 4;

//Potentiometer Pin on the ADC
const int PotiPin = 5;

//Light Depending Resistor Pins on the ADC
const int LCR0Pin = 0;
const int LCR1Pin = 1;
const int LCR2Pin = 2;
const int LCR3Pin = 3;

//Threshold Value for Automatic Mode
const int ThresholdValue = 20;

//Servo Steps for Automatic Mode
const int ServoSteps = 1;

bool prevButton = false;
bool currButton = false;


//==========|Custom Characters|==========
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

//==========|Setup Method|==========
void setup() {

  //Begin Serial Communication for Debugging
  Serial.begin(9600);

  //Beginn Serial Periferal Interface Communication
  SPI.begin();

  //Initialise LCD
  lcd.init();
  //Turn on lcd backlight
  lcd.backlight();

  //Attach Servos
  ServoLower.attach(ServoLowerPin);
  ServoUpper.attach(ServoUpperPin);

  //Set PinMode
  pinMode(MCP3008_cs, OUTPUT);
  pinMode(ModeButton, INPUT_PULLUP);

  //Set ChipSelect Pin of ADC High (Pin is active Low)
  digitalWrite(MCP3008_cs, HIGH);

  //Create Custom Characters
  lcd.createChar(0, ModeOffChar);
  lcd.createChar(1, ModeAutoChar);
  lcd.createChar(2, ModeLowerChar);
  lcd.createChar(3, ModeUpperChar);
  lcd.createChar(4, degree);

}

//==========|Setup Method|==========
void loop() {
  //Check if Button is Pressed
  currButton = digitalRead(ModeButton);
  if (currButton == false && prevButton == true)
  {
    Mode++;

    if (Mode > 3)
    {
      Mode = 0;
    }
  }
  prevButton = currButton;

  //Execute Depending on Mode
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

  //Set Servos to their values
  SetServos();
  //Write Information to LCD
  WriteInfoToLCD();
}

//==========|Mode Off Method|==========
void ModeOff()
{
  //Set both Servos to 90°
  ServoLowerValue = 90;
  ServoUpperValue = 90;
}

//==========|Mode Auto Method|==========
void ModeAuto()
{
    //Read values of LDRs
    int LDR0Value = mcp3008_read(LCR0Pin);
    int LDR1Value = mcp3008_read(LCR1Pin);
    int LDR2Value = mcp3008_read(LCR2Pin);
    int LDR3Value = mcp3008_read(LCR3Pin);

    //Compare LDR Values for Upper Servo
    if(LDR0Value - ThresholdValue > LDR2Value)
    {
      ServoUpperValue += ServoSteps;
    }
    else if(LDR2Value - ThresholdValue > LDR0Value)
    {
      ServoUpperValue -= ServoSteps;
    }

    //Compare LDR Values for Lower Servo
    if(LDR1Value - ThresholdValue > LDR3Value)
    {
      ServoLowerValue += ServoSteps;
    }
    else if(LDR3Value - ThresholdValue > LDR1Value)
    {
      ServoUpperValue -= ServoSteps;
    }
}

//==========|Mode Manual Lower Servo Method|==========
void ModeManLower()
{
  //Read Potentiometer Value
  int PotiValue = mcp3008_read(PotiPin);
  //Set Servo according to Potentiometer Value
  ServoLowerValue = map(PotiValue, 0, 1023, 0, 180);
}

//==========|Mode Manual Upper Servo Method|==========
void ModeManUpper()
{
  //Read Potentiometer Value
  int PotiValue = mcp3008_read(PotiPin);
  //Set Servo according to Potentiometer Value
  ServoUpperValue = map(PotiValue, 0, 1023, 0, 180);
}

//==========|Write Info to LCD Method|==========
void WriteInfoToLCD()
{
  //Read and convert Voltage of Solar Cell
  int VsolarValue = mcp3008_read(VsolarPin);
  VsolarValue = map(VsolarValue, 0, 1023, 0, 5000);

  //Write Mode
  WriteModeToLCD();

  //Write Lower Servo Angle
  lcd.setCursor(0, 1);
  lcd.print("SrvLwr: ");
  lcd.print(ServoLowerValue);
  lcd.write(4);
  lcd.print("   ");

  //Write Upper Servo Angle
  lcd.setCursor(0, 2);
  lcd.print("SrvUpr: ");
  lcd.print(ServoUpperValue);
  lcd.write(4);
  lcd.print("   ");

  //Write Voltage of Solar Cell
  lcd.setCursor(0, 3);
  lcd.print("Vsolar= ");
  lcd.print(VsolarValue);
  lcd.print("mV      ");
}

//==========|Set Servos|==========
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

  //Write Values to Servos
  ServoLower.write(ServoLowerValue);
  ServoUpper.write(ServoUpperValue);
}

//Read analog value from ADC (MCP3008)
uint16_t mcp3008_read(uint8_t channel) {
  //Set ChipSelect LOW
  Serial.print("Reading Pin: ");
  Serial:println(channel);
  digitalWrite(MCP3008_cs, LOW);

  //Start SPI Transfer
  SPI.transfer(0x01);

  //Transfer Value
  uint8_t msb = SPI.transfer(0x80 + (channel << 4));
  uint8_t lsb = SPI.transfer(0x00);

  Serial.print("MSB is: ");
  Serial:println(msb);
  Serial.print("LSB is: ");
  Serial:println(lsb);
  //Set ChipSelect HIGH
  digitalWrite(MCP3008_cs, HIGH);

  //Return read value
  return ((msb & 0x03) << 8) + lsb;
}


//==========|Write Mode to LCD Method|==========
void WriteModeToLCD()
{
  //Set Cursor Position to top left
  lcd.setCursor(0, 0);
  lcd.print("Mode: ");

  //Write Custom Character Depending on Mode
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
