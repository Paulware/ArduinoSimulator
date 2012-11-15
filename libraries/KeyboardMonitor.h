#ifndef KEYBOARDMONITOR_H
#define KEYBOARDMONITOR_H
#include "Component.h"
class KeyboardMonitor:Component
{
  public:          
    // Constructor
    KeyboardMonitor(); 
    HWND DrawWindow(char * title, HINSTANCE hInst, int x, int y);
    void Paint(HWND hWnd);
    void GetWindowTxt (  TCHAR * szText );
 
  private:
    HWND hWndEdit;
};

#endif


