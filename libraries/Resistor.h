#ifndef RESISTOR_H
#define RESISTOR_H
#include "ConnectedComponent.h"
class Resistor:public ConnectedComponent
{
  public:          
    // Constructor
    Resistor(int _x, int _y, int _resistance); 
    // Destructor
    ~Resistor();
    Pin * PortSelected ();    
    void Paint(HDC _hdc, PAINTSTRUCT _ps, HDC _hdcMemory);
    void HandleMouseDown (HWND hWnd, int _x, int _y);                      
    void HandleMouseMove (HWND hWnd, int _x, int _y);                      
    void HandleMouseUp (HWND hWnd); 
	void HandleMenu(int command){};
    void AddMenu ();       
    void MoveTo (int, int);  
    Pin * PinActive ();      
    bool IsSet();
    void SetPins();
    void Reset();
    void SaveYourself (FILE * fp);        
    void Select (bool){};
	void Init (HWND _windowHandle, HINSTANCE _g_hInst, char *);
    
    
    Pin * FindPort ( char * port);
    Pin * input;
    Pin * output;
    int resistance;
    
  private:
};

#endif


