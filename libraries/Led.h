#ifndef LED_H
#define LED_H
#include "Component.h"
#include "Pin.h"
class Led:Component
{
  public:          
    // Constructor
    Led(); 
    // Destructor
    ~Led();
    HINSTANCE g_hInst; 
    HWND DrawWindow (char * title, HINSTANCE hInst, int x , int y);   
    void Paint(HWND hWnd);                      
    Pin * gnd;
    Pin * power;
    
  private:
    bool offOn;
    HBITMAP       hbmRedDot;
    BITMAP        bmRedDot;

    HBITMAP       hbmBlackDot;
    BITMAP        bmBlackDot;
};

#endif


