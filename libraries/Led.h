#ifndef LED_H
#define LED_H
#include "Component.h"
#include "Pin.h"
class Led:public Component
{
  public:          
    Led(int,int); // Constructor
    ~Led(); // Destructor
    Pin * PortSelected ();
    void Select (bool);
    void PaintStart ( HDC & _hdcWindow, HDC & _hdcMemory, PAINTSTRUCT &_ps);
    void Paint(HWND hWnd);                      
    void HandleMenu ( int command );
    void HandleMouseMove (HWND hWnd, int _x, int _y);  
    void HandleMouseDown (HWND hWnd, int _x, int _y);  
    void HandleMouseUp (HWND hWnd);
    void Reset();
    bool IsSet();
    void MoveTo (int, int);  
    Pin * PinActive ();      
    void LoadBMap (char * bmpResource, HBITMAP &hBitMap, BITMAP &bitMap );
    Pin * gnd;
    Pin * power;
    void AddMenu (); 
	    
  private:
    bool offOn;
    HBITMAP       hbmRedDot;
    BITMAP        bmRedDot;

    HBITMAP       hbmBlackDot;
    BITMAP        bmBlackDot;
    
};

#endif


