#ifndef LIQUIDCRYSTAL_H
#define LIQUIDCRYSTAL_H
#include <Arduino.h>
class LiquidCrystal
{
  public:          
    // Constructor
    LiquidCrystal(){};
    LiquidCrystal(int,int,int,int,int,int) {}; 
	void setCursor(uint8_t _x, uint8_t _y ) {x =_x; y=_y;};
	void print (char * ch) {lcdPrint ( ch );};
	void clear(){lcdClear();};
    void begin(uint8_t cols, uint8_t rows, uint8_t charsize = 0){};
	
    
  private:
    uint8_t x;
	uint8_t y;
};

#endif


