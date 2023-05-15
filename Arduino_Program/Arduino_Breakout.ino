#include <SPI.h>
#include <LiquidCrystal_I2C.h>
//#inculde "Arduino_Breakout.h"

//==========|Custom Characters|==========

//Custom Chars for Ball
byte LeftUpper[] = {
  B11000,
  B11000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte RightUpper[] = {
  B00011,
  B00011,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte LeftLower[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11000,
  B11000
};
byte RightLower[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00011,
  B00011
};
byte Middle[] = {
  B00000,
  B00000,
  B00000,
  B00100,
  B00100,
  B00000,
  B00000,
  B00000
};

//Custom Chars for Ball with Bar
byte LeftUpperWithBar[] = {
  B11000,
  B11000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B00000
};
byte RightUpperWithBar[] = {
  B00011,
  B00011,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B00000
};
byte LeftLowerWithBar[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B11000,
  B11000,
  B11111,
  B00000
};
byte RightLowerWithBar[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00011,
  B00011,
  B11111,
  B00000
};
byte MiddleWithBar[] = {
  B00000,
  B00000,
  B00000,
  B00100,
  B00100,
  B00000,
  B11111,
  B00000
};

void Breakout()
{
  //Initialise Custom Chars
  Breakout_InitCustomChars();
  lcd.createChar(9, FullBlock);

  lcd.clear();

  randomSeed(analogRead(A0));

  byte prevBarPos = 0;
  byte barPos = 0;
  byte loopCounter = 0;
  byte Speed = 5; //Speed, must be smaller then 256
  byte prevBallPosX = 0;
  byte prevBallPosY = 0;
  byte BallPosX = random(9, 12);
  byte BallPosY = 2;
  byte BallMovement = random(0, 4) * 5 + 2;
  uint16_t Score = 0;
  int64_t bricks = 1099511627775; //Each brick represents a bit in this int
  /* //Movement + in Cell Pos
   * 0 - 4 = LeftUpwards Movement
   * 5 - 9 = RightUpwards Movement
   * 10 - 14 = LeftDownwards Movement
   * 15 - 19 = RightDownwards Movement
   * 
   * (BallMovement + 1) / 4: Movement
   * 
   * BallMovement % 5 == 0: LeftUpper in Cell Pos
   * BallMovement % 5 == 1: RightUpper in Cell Pos
   * BallMovement % 5 == 2: Middle in Cell Pos
   * BallMovement % 5 == 3: LeftLower in Cell Pos
   * BallMovement % 5 == 4: RightLower in Cell Pos
   */

  //==========|Initialise Board|==========
  Breakout_DrawBricks(bricks);

  
  //==========|Main Game Loop|==========
  while(digitalRead(ModeButton))
  {
    //Update Prev Positions
    prevBallPosX = BallPosX;
    prevBallPosY = BallPosY;
    prevBarPos = barPos;

    //Update Bar Position
    barPos = map(mcp3008_read(PotiPin), 0, 1023, 0, 18);

    //Redraw Bar if its position has changed
    if(prevBarPos != barPos)
    {
      //Clear Prev Pos
      lcd.setCursor(prevBarPos, 3);
      if(prevBarPos == BallPosX && BallPosY == 3)
      {
        lcd.write((BallMovement % 5) + 4);
        lcd.print(F(" "));
      }
      else if(prevBarPos + 1 == BallPosX && BallPosY == 3)
      {
        lcd.print(F(" "));
        lcd.write((BallMovement % 5) + 4);
      }
      else
      {
        lcd.print(F("  "));
      }

      //Redraw at new Pos
      lcd.setCursor(barPos, 3);
      //Print Bar
      if(barPos == BallPosX && BallPosY == 3)
      {
        lcd.write((BallMovement % 5) + 4);
        lcd.print(F("_"));
      }
      else if(barPos + 1 == BallPosX && BallPosY == 3)
      {
        lcd.print(F("_"));        
        lcd.write((BallMovement % 5) + 4);
      }
      else
      {
        lcd.print(F("__"));
      }
    }
    
    //Move Ball every Speed's loop
    if(loopCounter % Speed == 0)
    {
      loopCounter = 0;
      
      //Ugly Ass Big Switch Case
      switch(BallMovement)
      {
        case 0:
          BallPosX -= 1;
          BallPosY -= 1;
          BallMovement = 4;
          break;
        case 1:
          BallPosY -= 1;
          BallMovement = 2;
          break;
        case 2:
          BallMovement = 0;
          break;
        case 3:
          BallPosX -= 1;
          BallMovement = 2;
          break;
        case 4:
          BallMovement = 2;
          break;
        case 5:
          BallPosY -= 1;
          BallMovement = 7;
          break;
        case 6:
          BallPosX += 1;
          BallPosY -= 1;
          BallMovement = 8;
          break;
        case 7:
          BallMovement = 6;
          break;
        case 8:
          BallMovement = 7;
          break;
        case 9:
          BallPosX += 1;
          BallMovement = 8;
          break;
        case 10:
          BallPosX -= 1;
          BallPosY += 1;
          BallMovement = 11;
          break;
        case 11:
          BallMovement = 12;
          break;
        case 12:
          BallMovement = 13;
          break;
        case 13:
          BallPosX -= 1;
          BallPosY += 1;
          BallMovement = 11;
          break;
        case 14:
          BallPosY += 1;
          BallMovement = 11;
          break;
        case 15:
          BallMovement = 17;
          break;
        case 16:
          BallPosX += 1;
          BallMovement = 17;
          break;
        case 17:
          BallMovement = 19;
          break;
        case 18:
          BallPosY += 1;
          BallMovement = 17;
          break;
        case 19:
          BallPosX += 1;
          BallPosY += 1;
          BallMovement = 15;
          break;
        default: //Print error if Unexpected BallMovement Occurres
          lcd.print(F("ERR BLMV"));
      }

      //Check if Brick was broken
      if(BallPosY == 0)
      {
        if(bitRead(bricks, BallPosX) == 1)
        {
          bitWrite(bricks, BallPosX, 0);
          BallMovement = Breakout_DeflectBallOnBrickHit(BallMovement, BallPosX, BallPosY);
          BallPosY++;
        }
      }
      else if(BallPosY == 1)
      {
        if(bitRead(bricks, BallPosX + 20) == 1)
        {
          bitWrite(bricks, BallPosX + 20, 0);
          BallMovement = Breakout_DeflectBallOnBrickHit(BallMovement, BallPosX, BallPosY);
          BallPosY++;
        }
      }

      //Reverse Ball Movement on Edges
      if(BallPosX > 19 && BallPosX < 150)
      {
        BallPosX = 19;
        if(BallMovement < 10)
        {
          BallMovement = 2;
        }
        else
        {
          BallMovement = 12;
        }
      }
      else if(BallPosX > 149)
      {
        BallPosX = 0;
        if(BallMovement < 9)
        {
          BallMovement = 7;
        }
        else
        {
          BallMovement = 12;
        }
      }
      if(BallPosY > 3 && BallPosY < 150)
      {
        if(BallPosX == barPos || BallPosX == barPos + 1)
        {
          BallPosY = 3;
          if(BallMovement < 15)
          {
            BallMovement = 2;
          }
          else
          {
            BallMovement = 7;
          }
        }
        //Reset the ball when the bar was not hit
        else
        {
          Score++;
          if(Score % 10 == 0)
          {
            if(Speed > 1)
            {     
              Speed--; 
            }
            Breakout_DrawBricks(bricks);
          }
          BallPosX = random(9, 12);
          BallPosY = 2;
          BallMovement = random(0, 4) * 5 + 2;
        }
      }
      else if(BallPosY > 149)
      {
        BallPosY = 0;
        if(BallMovement < 5)
        {
          BallMovement = 12;
        }
        else
        {
          BallMovement = 17;
        }
      }

      //Draw Ball with Bar if applicable
      if(BallPosY == 3 && (BallPosX == barPos || BallPosX == barPos + 1))
      {
        Breakout_InitCustomCharsWithBar();
      }
      else
      {
        Breakout_InitCustomChars();
      }
      
      //Draw Ball
      if(prevBallPosX != BallPosX || prevBallPosY != BallPosY)
      {
        if((prevBallPosY == 3 && prevBallPosX == barPos) || (prevBallPosY == 3 && prevBallPosX == barPos + 1))
        {
          lcd.setCursor(prevBallPosX, prevBallPosY);
          lcd.print(F("_"));           
        }
        else
        {
          lcd.setCursor(prevBallPosX, prevBallPosY);
          lcd.print(F(" "));
        }
      }
      lcd.setCursor(BallPosX, BallPosY);
      lcd.write((BallMovement % 5) + 4);
    }
    loopCounter++;

    //Break out of loop if all bricks are cleared
    if(bricks == 0)
    {
      break;
    }
  }
  lcd.setCursor(3, 1);
  lcd.print(F("You're Winner!"));
  lcd.setCursor(4, 2);
  lcd.print(F("Score: "));
  lcd.print(65536 - Score);

  //Reset Custom Chars
  lcd.createChar(4, degree);
  while(true)
  {
    //Do nothing    
  }
}
void Breakout_InitCustomCharsWithBar()
{
  lcd.createChar(4, LeftUpperWithBar);
  lcd.createChar(5, RightUpperWithBar);
  lcd.createChar(6, MiddleWithBar);
  lcd.createChar(7, LeftLowerWithBar);
  lcd.createChar(8, RightLowerWithBar);
}

void Breakout_InitCustomChars()
{
  lcd.createChar(4, LeftUpper);
  lcd.createChar(5, RightUpper);
  lcd.createChar(6, Middle);
  lcd.createChar(7, LeftLower);
  lcd.createChar(8, RightLower);
}

byte Breakout_DeflectBallOnBrickHit(byte BallMovement, byte BallPosX, byte BallPosY)
{
  lcd.setCursor(BallPosX, BallPosY);
  lcd.print(F(" "));
  if(BallMovement < 5)
  {
    return 12;
  }
  else
  {
    return 17;
  }
}

void Breakout_DrawBricks(int64_t bricks)
{
  lcd.setCursor(0, 0);
  for(int i = 0; i < 20; i++)
  {
    if(bitRead(bricks, i) == 1)
    {
     lcd.write(9); 
    }
    else
    {
      lcd.print(F(" "));
    }
  }
  lcd.setCursor(0, 1);
  for(int i = 0; i < 20; i++)
  {
    if(bitRead(bricks, i + 20) == 1)
    {
     lcd.write(9); 
    }
    else
    {
      lcd.print(F(" "));
    }
  }
}