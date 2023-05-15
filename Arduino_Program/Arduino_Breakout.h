#ifndef ARDUINO_BIRD_H
#define ARDUINO_BIRD_H
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

void Breakout();
void Breakout_InitCustomCharsWithBar();
void Breakout_InitCustomChars();
byte Breakout_DeflectBallOnBrickHit(byte BallMovement, byte BallPosX, byte BallPosY);
void Breakout_DrawBricks(int64_t bricks);

#endif