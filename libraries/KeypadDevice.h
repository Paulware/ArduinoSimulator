#ifndef KEYPADDEVICE_H
#define KEYPADDEVICE_H
#include "ConnectedComponent.h"
class KeypadDevice:public ConnectedComponent
{
  public:          
    // Constructor
    KeypadDevice(int, int); 
    // Destructor
    ~KeypadDevice();
    void PaintStart ( HDC & _hdcWindow, HDC & _hdcMemory, PAINTSTRUCT &_ps);
    void Paint(HWND hWnd);  
    void LoadBMap (char * bmpResource, HBITMAP &hBitMap, BITMAP &bitMap );	 
    bool clearTheText;
    void HandleMenu ( int command );
    void AddMenu ();     
    void HandleMouseMove (HWND hWnd, int _x, int _y);  
    void HandleMouseDown (HWND hWnd, int _x, int _y); 
    void HandleMouseUp (HWND hWnd);
    
    void MoveTo (int, int); 
	Pin * PinActive(); 
    Pin * PortSelected();
    void SaveYourself (FILE * fp);
    void NotSavedYet(){}; 
	void Select (bool);  
    bool IsSet();
    void Reset();
    Pin * FindPort ( char * port);
    static char keypadChars[16];  
	static char getKey;  
	 
  private:
  	
	static int const MAX_KEYPAD_PINS = 8;
    Pin * pin[MAX_KEYPAD_PINS];
    
    static int const MAX_KEYPADS = 16;
    COORD pads[MAX_KEYPADS];
    
    
    HBITMAP       hbmPinDot;
    BITMAP        bmPinDot;
    
    HBITMAP       hbmRedDot;
    BITMAP        bmRedDot;

    HBITMAP       hbmBlackDot;
    BITMAP        bmBlackDot;
};

#endif


