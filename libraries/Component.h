#ifndef COMPONENT_H
#define COMPONENT_H
#include <windows.h>
class Component
{
  public:
    // Constructor
    Component();
    HWND DrawWindow (char * title, HINSTANCE hInst, char * bmpResource, 
                     int x , int y, int width, int height);
    void Paint (HWND, HDC &hdcWindow, HDC &hdcMemory, PAINTSTRUCT &ps);
    void PaintEnd ( HDC * hdcMemory, HWND hWnd, PAINTSTRUCT * ps);    
    void LoadBMap (char * bmpResource, HBITMAP &hBitMap, BITMAP &bitMap );
    void Refresh ();
    
  protected:    
    HINSTANCE g_hInst;    
    HWND   windowHandle;
    HWND   lastWindow;
    
  private:
    HBITMAP hbm;
    BITMAP bm;
    bool bitmapExists;
};
#endif


