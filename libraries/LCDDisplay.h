#ifndef LCDDISPLAY_H
#define LCDDISPLAY_H
#include "Component.h"
#include "Pin.h"
#define MAX_LCD_PINS 16

class LCDDisplay:public Component
{
  public:          
    // Constructor
    LCDDisplay(int, int); 
    // Destructor
    ~LCDDisplay();
    void PaintStart ( HDC & _hdcWindow, HDC & _hdcMemory, PAINTSTRUCT &_ps);
    void Paint(HWND hWnd);  
    void LoadBMap (char * bmpResource, HBITMAP &hBitMap, BITMAP &bitMap );	 
    void print ( char * ch );                   
    void clear(HDC);
    bool clearTheText;
    void HandleMenu ( int command );
    void AddMenu ();     
    void HandleMouseMove (HWND hWnd, int _x, int _y);  
    void HandleMouseDown (HWND hWnd, int _x, int _y);  
    void MoveTo (int, int); 
	Pin * PinActive(); 
    Pin * PortSelected();
  
  private:
    bool offOn;
    char screen[80];
    void showScreen(HDC hdcWindow);
    Pin * pin[MAX_LCD_PINS];
    
    HBITMAP       hbmPinDot;
    BITMAP        bmPinDot;
    
    HBITMAP       hbmRedDot;
    BITMAP        bmRedDot;

    HBITMAP       hbmBlackDot;
    BITMAP        bmBlackDot;
};

#endif


