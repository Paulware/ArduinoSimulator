#ifndef ARDUINOCOMPONENT_H
#define ARDUINOCOMPONENT_H
#include <windows.h>
#include "ConnectedComponent.h"
#define MAX_DIGITAL_VALUES 20
class ArduinoComponent:public ConnectedComponent
{
  public:          
    // Constructor
    ArduinoComponent(int, int); 
    // Destructor
    ~ArduinoComponent();
    Pin * power;
	Pin * gnd;
	Pin * d[MAX_DIGITAL_VALUES];
	Pin * GetConnection (int);
    void Select (bool);
    void LoadBMap (char * bmpResource, HBITMAP &hBitMap, BITMAP &bitMap );    
    void PaintStart ( HDC & _hdcWindow, HDC & _hdcMemory, PAINTSTRUCT &_ps);
    void Paint (HWND hWnd);    
    void digitalWrite (int, int); 
    void MoveTo (int _x, int _y); 
    void AddMenu ();    
    void HandleMouseDown (HWND hWnd, int _x, int _y); 
    void HandleMouseMove (HWND hWnd, int _x, int _y);    
    void SaveYourself (FILE * fp);         
    Pin * FindPort ( char *); 
    bool IsSet();
    void Reset();	   
        
    Pin * PortSelected();
    Pin * PinActive();	  
    
  private:
    int redDotX,redDotY,blackDotX,blackDotY;

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


