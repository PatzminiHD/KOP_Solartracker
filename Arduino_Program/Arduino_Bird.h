#ifndef ARDUINO_BIRD_H
#define ARDUINO_BIRD_H
#include <LiquidCrystal_I2C.h>
#include <Arduino.h> 

void Bird();
bool Bird_IsNotColliding(byte BirdPos, byte Pipe0X, byte Pipe0Height, byte Pipe1X, byte Pipe1Height);
void Bird_DrawScore(int score);
void Bird_DrawPipes(byte Pipe0X, byte Pipe0Height, byte Pipe1X, byte Pipe1Height);
void Bird_ClearLCDColumn(byte Column);
void Bird_SetPipeChars(byte PipeHeight, byte PipeNumber);
void Bird_DrawBird(byte BirdPos);
void Bird_SetBirdChars(byte BirdPos);

#endif