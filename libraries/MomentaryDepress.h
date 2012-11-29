#ifndef MOMENTARYDEPRESS_H
#define MOMENTARYDEPRESS_H
#include "Component.h"
#include "Pin.h"
class MomentaryDepress:public Component
{
  public:          
    // Constructor
    MomentaryDepress(int _x, int _y); 
    // Destructor
    ~MomentaryDepress();
    Pin * PortSelected ();    
    void PaintStart ( HDC & _hdcWindow, HDC & _hdcMemory, PAINTSTRUCT &_ps);
    void Paint(HWND hWnd);
    void HandleMouseDown (HWND hWnd, int _x, int _y);                      
    void HandleMouseMove (HWND hWnd, int _x, int _y);                      
    void HandleMouseUp (HWND hWnd); 
	void HandleMenu(int command){};
    void AddMenu ();       
    void MoveTo (int, int);  
    Pin * PinActive ();      
    void LoadBMap (char * bmpResource, HBITMAP &hBitMap, BITMAP &bitMap);
    bool IsSet();
    void Reset();

    Pin * input;
    Pin * output;
    
  private:
    bool depressed;
    HBITMAP       hbmDepressed;
    BITMAP        bmDepressed;
};

#endif


