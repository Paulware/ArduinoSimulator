#ifndef LED_H
#define LED_H
#include "ConnectedComponent.h"
class Led:public ConnectedComponent
{
  public:          
    Led(int,int);
    ~Led();
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
    void SaveYourself (FILE * fp);    
    Pin * FindPort ( char *);    
	 	    
  private:
    bool offOn;
    HBITMAP       hbmRedDot;
    BITMAP        bmRedDot;

    HBITMAP       hbmBlackDot;
    BITMAP        bmBlackDot;
    
};

#endif


