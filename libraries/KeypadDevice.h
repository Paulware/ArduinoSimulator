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
    void Paint(HDC _hdc, PAINTSTRUCT _ps, HDC _hdcMemory);  
    void Init (HWND _windowHandle, HINSTANCE _g_hInst, char *);
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
    bool TroubleshootPins ( );

    void static ConnectPin (byte pin);
	static int const MAX_KEYPAD_PINS = 8;
    static int const MAX_KEYPADS = 16;
    static char keypadChars[MAX_KEYPADS];
	static char getKey;
	static int pinIndex;
	static byte connectedTo[MAX_KEYPAD_PINS];
	 
  private:
  	
    Pin * pin[MAX_KEYPAD_PINS];
    
    COORD pads[MAX_KEYPADS];
    
    
    HBITMAP       hbmPinDot;
    BITMAP        bmPinDot;
    
    HBITMAP       hbmRedDot;
    BITMAP        bmRedDot;

    HBITMAP       hbmBlackDot;
    BITMAP        bmBlackDot;
};

#endif


