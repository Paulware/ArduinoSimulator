#ifndef ARDUINOCOMPONENT_H
#define ARDUINOCOMPONENT_H
#include <windows.h>
#include "Component.h"
#include "Pin.h"
#define MAX_DIGITAL_VALUES 20
class ArduinoComponent:Component
{
  public:          
    // Constructor
    ArduinoComponent(); 
    // Destructor
    ~ArduinoComponent();
    Pin * power;
	Pin * gnd;
	Pin * d[MAX_DIGITAL_VALUES];
    HWND DrawWindow (char * title, HINSTANCE hInst, int x , int y);
    void Paint (HWND);    
    void digitalWrite (int, int); 
    HINSTANCE g_hInst; 
    HDC hdc;
    HWND formHandle;
    
  private:
    int x,y,redDotX,redDotY,blackDotX,blackDotY;

    HBITMAP       hbmRedDot;
    BITMAP        bmRedDot;

    HBITMAP       hbmBlackDot;
    BITMAP        bmBlackDot;
    
    struct digitalInfo
    {
      int x;
      int y;
      bool blackRed;     
    };
    digitalInfo digitalValues[MAX_DIGITAL_VALUES];    

};

#endif


