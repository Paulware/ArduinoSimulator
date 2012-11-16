#ifndef MOMENTARYDEPRESS_H
#define MOMENTARYDEPRESS_H
#include "Component.h"
#include "Pin.h"
class MomentaryDepress:Component
{
  public:          
    // Constructor
    MomentaryDepress(); 
    // Destructor
    ~MomentaryDepress();
    HINSTANCE g_hInst; 
    HWND DrawWindow (char * title, HINSTANCE hInst, int x , int y);   
    void Paint(HWND hWnd);
    void HandleMouseDown (HWND hWnd);                      
    void HandleMouseUp (HWND hWnd);                      
    Pin * input;
    Pin * output;
    
  private:
    bool depressed;
    HBITMAP       hbmDepressed;
    BITMAP        bmDepressed;
};

#endif


