#ifndef LCDDISPLAY_H
#define LCDDISPLAY_H
#include "Component.h"
#include "Pin.h"

class LCDDisplay:Component
{
  public:          
    // Constructor
    LCDDisplay(); 
    // Destructor
    ~LCDDisplay();
    HINSTANCE g_hInst; 
    HWND DrawWindow (char * title, HINSTANCE hInst, int x , int y);   
    void Paint(HWND hWnd);   
    void print ( char * ch );                   
    void clear(HDC);
    bool clearTheText;
    
  private:
    bool offOn;
    int x;
    int y;
    char screen[80];
    void showScreen(HDC hdcWindow);
};

#endif


