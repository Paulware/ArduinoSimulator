#ifndef MOMENTARYDEPRESS_H
#define MOMENTARYDEPRESS_H
#include "ConnectedComponent.h"
class MomentaryDepress:public ConnectedComponent
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
    void SetPins();
    void Reset();
    void Select (bool){};    
    void SaveYourself (FILE * fp);        
    Pin * FindPort ( char * port);
    Pin * input;
    Pin * output;
    
  private:
    bool depressed;
    HBITMAP       hbmDepressed;
    BITMAP        bmDepressed;
};

#endif


