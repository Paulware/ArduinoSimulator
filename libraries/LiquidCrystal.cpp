#include "LiquidCrystal.h"

LiquidCrystal::LiquidCrystal()
{ 
}

LiquidCrystal::LiquidCrystal(int a,int b,int c,int d,int e,int f)
{
} 

void LiquidCrystal::setCursor(uint8_t _x, uint8_t _y )
{
 x =_x; 
 y=_y;
}
