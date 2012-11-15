#ifndef ARDUINOCOMPONENT_H
#define ARDUINOCOMPONENT_H
#include <windows.h>
#include "Component.h"
#include "Pin.h"
class ArduinoComponent:Component
{
  public:          
    // Constructor
    ArduinoComponent(); 
    // Destructor
    ~ArduinoComponent();
	Pin * gnd;
	Pin * d[20];
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
    digitalInfo digitalValues[14];    
    char   BCX_ClassName[2048];  // default size

};

#endif


