#include <LiquidCrystal_I2C.h>
//#inculde "Arduino_Bird.h"


//==========|Custom Characters|==========
const byte BirdChar_Pos0[] = {
  B01110,
  B01111,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
const byte BirdChar_Pos1[] = {
  B00000,
  B01110,
  B01111,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000
};
const byte BirdChar_Pos2[] = {
  B00000,
  B00000,
  B01110,
  B01111,
  B01110,
  B00000,
  B00000,
  B00000
};
const byte BirdChar_Pos3[] = {
  B00000,
  B00000,
  B00000,
  B01110,
  B01111,
  B01110,
  B00000,
  B00000
};
const byte BirdChar_Pos4[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B01110,
  B01111,
  B01110,
  B00000
};
const byte BirdChar_Pos5[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B01110,
  B01111,
  B01110
};
const byte BirdChar_Pos6_Top[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B01110,
  B01111
};
const byte BirdChar_Pos7_Top[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B01110
};
const byte BirdChar_Pos7_Bot[] = {
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
const byte BirdChar_Pos8_Bot[] = {
  B01111,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

const byte Pipe_Top_ThreeQuarters[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B00000,
  B00000
};
const byte Pipe_Bot_ThreeQuarters[] = {
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
const byte Pipe_Top_Quarter[] = {
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
const byte Pipe_Bot_Quarter[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111
};
const byte Pipe_Top_Half[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000
};
const byte Pipe_Bot_Half[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111
};

const byte FullBlock[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
const byte EmptyChar[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

//==========|Main Method|==========
void Bird()
{
  byte BirdPos = 16;        //Vertical Position of Bird
  byte ProgSpeed = 50;      //Speed of the program (Higher Number -> Slower)
  byte Gravity = 1;         //Gravity in Pos/tick^2
  char BirdSpeed = 0;       //Speed in Pos/tick
  byte JumpStrength = 4;    //JumpStrength in Pos/tick
  byte JumpAndNewPipe = 3;  //LSB = Jump; LSB << 1 = allowJump; LSB << 2 = allowSecondPipe
  byte Bird_Score = 0;      //Score, increments every pipe
  byte Pipe0X = 19;         //Horizontal Position of Pipe 0
  byte Pipe0Height = 1;     //Height of Pipe 0
  byte Pipe1X = 19;         //Horizontal Position of Pipe 0
  byte Pipe1Height = 6;     //Height of Pipe 1
  unsigned long mills = 0;  //Helper variable, stores milliseconds the program has run

  lcd.clear();
  randomSeed(analogRead(A0));     //Initialise the random seed
  lcd.createChar(6, FullBlock);
  //==========|Main Game loop|==========
  //Running as long as the bird is not colliding with a pipe
  while(Bird_IsNotColliding(BirdPos, Pipe0X, Pipe0Height, Pipe1X, Pipe1Height))
  {
    //Print vars for debugging
    /*lcd.setCursor(5, 0);
    lcd.print(BirdPos);
    lcd.setCursor(5, 1);
    lcd.print(Jump);*/

    //Increment score if Bird is at the same horizontal position as a pipe
    if(Pipe0X == 2 || Pipe1X == 2)
    {
      Bird_Score++;
    }

    //Move Pipe 0 to the left
    Pipe0X--;

    //Check if the Pipe 0 position has underflown
    if(Pipe0X > 40)
    {
      Pipe0X = 19;                //Move the pipe to the right
      Pipe0Height = random(0, 6); //Set a new random height
    }

    //Check if the Pipe 1 has been enabled
    if(bitRead(JumpAndNewPipe, 2))
    {
      Pipe1X--;                   //Move the pipe to the right
    }

    //Check if the Pipe 0 is in the middle of the screen
    if(Pipe0X == 9)
    {
      Pipe1X = 19;                    //Move the pipe to the right
      Pipe1Height = random(0, 6);     //Set a new random height
      bitWrite(JumpAndNewPipe, 2, 1); //Enable the pipe
    }

    //Draw the pipes
    Bird_DrawPipes(Pipe0X, Pipe0Height , Pipe1X, Pipe1Height);
    //Draw the bird
    Bird_DrawBird(BirdPos);
    //Draw the score
    Bird_DrawScore(Bird_Score);

    //Update Position
    BirdPos += BirdSpeed;
    //Update Speed
    BirdSpeed += Gravity;

    //Reset the jump bit
    bitWrite(JumpAndNewPipe, 0, 0);
    //Reset the millis
    mills = millis();

    //Run until the loop has run for ProgSpeed milliseconds
    while(mills + ProgSpeed > millis())
    {
      //Check if the button has been pressed
      if(digitalRead(ModeButton))
      {
        bitWrite(JumpAndNewPipe, 1, 1); //Set the jump bit
      }
      //Check if button is not pressed and jump is allowed
      if(!digitalRead(ModeButton) && bitRead(JumpAndNewPipe, 1))
      {
        bitWrite(JumpAndNewPipe, 1, 0); //Disallow jump
        bitWrite(JumpAndNewPipe, 0, 1); //Set the jump bit
      } 
    }

    //Check if jump is set
    if(bitRead(JumpAndNewPipe, 0))
    {
      bitWrite(JumpAndNewPipe, 0, 0);   //Reset jump bit
      BirdSpeed = -JumpStrength;        //Set bird speed to the JumpStrength
    }

    //If Bird Position is higher than 32
    if(BirdPos > 32 && BirdPos < 128)
    {
      BirdPos = 32;   //Limit to 32
      BirdSpeed = 0;  //Reset Bird speed
    }
    //If Bird Position has underflown
    else if (BirdPos >= 128)
    {
      BirdPos = 0;    //Set back to 0
    }
    //Set max Speed
    if(BirdSpeed > 5)
    {
      BirdSpeed = 5;
    }
  }

  //Print score if game is over
  lcd.setCursor(5, 1);
  lcd.print(F("Score: "));
  lcd.print(Bird_Score);

  //Wait for button to be pressed and unpressed to continue
  while(digitalRead(ModeButton)) {}
  while(!digitalRead(ModeButton)) {}
  
  lcd.clear();

}

//==========|Test if Bird is colliding|==========
bool Bird_IsNotColliding(byte BirdPos, byte Pipe0X, byte Pipe0Height, byte Pipe1X, byte Pipe1Height)
{
  //Set the Height variable if a pipe is an the same X coordinate as the bird
  byte Height = 0;
  if(Pipe0X == 2)
  {
    Height = Pipe0Height;
  }
  else if(Pipe1X == 2)
  {
    Height = Pipe1Height;
  }
  //Return if no pipe is at the same horizontal position as bird
  else
  {
    return true;
  }

  //Translate Pipe Height to Bird Height
  switch(Height)
  {
    case 0:
      Height = 1;
      break;
    case 1:
      Height = 3;
      break;
    case 2:
      Height = 5;
      break;
    case 3:
      Height = 7;
      break;
    case 4:
      Height = 10;
      break;
    case 5:
      Height = 12;
      break;
    case 6:
      Height = 14;
      break;
    case 7:
      Height = 16;
      break;
  }

  //Check if pipe and bird are colliding
  if(BirdPos > Height && BirdPos < Height + 17)
  {
    return true;
  }
  else
  {
    return false;
  }
}

//==========|Draw Score|==========
void Bird_DrawScore(int score)
{
  //Check for length of score and draw score to the right upper corner
  if(score < 10)
  {
    lcd.setCursor(19, 0);
    lcd.print(score);
  }
  else if(score < 100)
  {
    lcd.setCursor(18, 0);
    lcd.print(score);
  }
  else if(score < 1000)
  {
    lcd.setCursor(17, 0);
    lcd.print(score);
  }
}

//==========|Draw Pipes|==========
void Bird_DrawPipes(byte Pipe0X, byte Pipe0Height, byte Pipe1X, byte Pipe1Height)
{
  //Set Custom Pipe Characters
  Bird_SetPipeChars(Pipe0Height, 0);
  Bird_SetPipeChars(Pipe1Height, 1);

  //==========|Draw Pipe 1|==========
  if(Pipe1Height < 19)
  {
   Bird_ClearLCDColumn(Pipe1X + 1); //Clear the column where the pipe has been
  }
  //If Pipe height is less than 4, draw the full block at the bottom
  if(Pipe1Height < 4)
  {
    lcd.setCursor(Pipe1X, 0);
    lcd.write(4);
    lcd.setCursor(Pipe1X, 2);
    lcd.write(5);
    lcd.setCursor(Pipe1X, 3);
    lcd.write(6);
  }
  //Else, draw the full block at the top
  else
  {
    lcd.setCursor(Pipe1X, 0);
    lcd.write(6);
    lcd.setCursor(Pipe1X, 1);
    lcd.write(4);
    lcd.setCursor(Pipe1X, 3);
    lcd.write(5);
  }

  //==========|Draw Pipe 0|==========
  if(Pipe0Height < 19)
  {
   Bird_ClearLCDColumn(Pipe0X + 1); //Clear the column where the pipe has been
  }
  //If Pipe height is less than 4, draw the full block at the bottom
  if(Pipe0Height < 4)
  {
    lcd.setCursor(Pipe0X, 0);
    lcd.write(2);
    lcd.setCursor(Pipe0X, 2);
    lcd.write(3);
    lcd.setCursor(Pipe0X, 3);
    lcd.write(6);
  }
  //Else, draw the full block at the top
  else
  {
    lcd.setCursor(Pipe0X, 0);
    lcd.write(6);
    lcd.setCursor(Pipe0X, 1);
    lcd.write(2);
    lcd.setCursor(Pipe0X, 3);
    lcd.write(3);
  }
}

//==========|Clear a column of the lcd|==========
void Bird_ClearLCDColumn(byte Column)
{
  lcd.setCursor(Column, 0);
  lcd.print(F(" "));
  lcd.setCursor(Column, 1);
  lcd.print(F(" "));
  lcd.setCursor(Column, 2);
  lcd.print(F(" "));
  lcd.setCursor(Column, 3);
  lcd.print(F(" "));
}

//==========|Set the custom pipe Chars|==========
void Bird_SetPipeChars(byte PipeHeight, byte PipeNumber)
{
  //Check the pipe number
  switch(PipeNumber)
  {
    case 0:
      //Check the pipe height
      switch(PipeHeight % 4)
      {
        case 0:
          lcd.createChar(2, Pipe_Top_Quarter);
          lcd.createChar(3, Pipe_Bot_ThreeQuarters);
          break;
        case 1:
          lcd.createChar(2, Pipe_Top_Half);
          lcd.createChar(3, Pipe_Bot_Half);
          break;
        case 2:
          lcd.createChar(2, Pipe_Top_ThreeQuarters);
          lcd.createChar(3, Pipe_Bot_Quarter);
          break;
        case 3:
          lcd.createChar(2, FullBlock);
          lcd.createChar(3, EmptyChar);
          break;
      }
      break;

    case 1:
      //Check the pipe height
      switch(PipeHeight % 4)
      {
        case 0:
          lcd.createChar(4, Pipe_Top_Quarter);
          lcd.createChar(5, Pipe_Bot_ThreeQuarters);
          break;
        case 1:
          lcd.createChar(4, Pipe_Top_Half);
          lcd.createChar(5, Pipe_Bot_Half);
          break;
        case 2:
          lcd.createChar(4, Pipe_Top_ThreeQuarters);
          lcd.createChar(5, Pipe_Bot_Quarter);
          break;
        case 3:
          lcd.createChar(4, FullBlock);
          lcd.createChar(5, EmptyChar);
          break;
      }
      break;
  }
}

//==========|Draw the bird|==========
void Bird_DrawBird(byte BirdPos)
{
  //Clear the Bird Column
  lcd.setCursor(2, 0);
  lcd.print(F(" "));
  lcd.setCursor(2, 1);
  lcd.print(F(" "));
  lcd.setCursor(2, 2);
  lcd.print(F(" "));
  lcd.setCursor(2, 3);
  lcd.print(F(" "));
  //Set the custom Bird Chars
  Bird_SetBirdChars(BirdPos);
  //Set the cursor to the char where the bird is
  lcd.setCursor(2, BirdPos / 9);
  //Draw the birds upper part
  lcd.write(0);
  //Draw the birds lower part, if it is between 2 characters
  if(BirdPos < 27)
  {
    lcd.setCursor(2, (BirdPos / 9) + 1);
    lcd.write(1);
  }
}

//==========|Set custom Bird Chars|==========
void Bird_SetBirdChars(byte BirdPos)
{
  //Check on the bird position and set custom chars accordingly
  switch(BirdPos % 9)
  {
    case 0:
      lcd.createChar(0, BirdChar_Pos0);
      lcd.createChar(1, EmptyChar);
      break;
    case 1:
      lcd.createChar(0, BirdChar_Pos1);
      lcd.createChar(1, EmptyChar);
      break;
    case 2:
      lcd.createChar(0, BirdChar_Pos2);
      lcd.createChar(1, EmptyChar);
      break;
    case 3:
      lcd.createChar(0, BirdChar_Pos3);
      lcd.createChar(1, EmptyChar);
      break;
    case 4:
      lcd.createChar(0, BirdChar_Pos4);
      lcd.createChar(1, EmptyChar);
      break;
    case 5:
      lcd.createChar(0, BirdChar_Pos5);
      lcd.createChar(1, EmptyChar);
      break;
    case 6:
      lcd.createChar(0, BirdChar_Pos6_Top);
      lcd.createChar(1, EmptyChar);
      break;
    case 7:
      lcd.createChar(0, BirdChar_Pos7_Top);
      lcd.createChar(1, BirdChar_Pos7_Bot);
      break;
    case 8:
      lcd.createChar(0, EmptyChar);
      lcd.createChar(1, BirdChar_Pos8_Bot);
      break;
  }
}
