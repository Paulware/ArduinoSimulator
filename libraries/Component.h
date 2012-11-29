#ifndef COMPONENT_H
#define COMPONENT_H
#include <windows.h>
#include "Menus.h"
class Component
{
  public:
    // Constructor
    Component();
    // Next function is virtual so that the matching component will call DrawWindow
    virtual HWND DrawWindow (char * title, HINSTANCE hInst, char * bmpResource, 
                             int x , int y, int width=0, int height=0);
	virtual int NumConnections(){return 0;};	  
    virtual void PaintStart (HDC & _hdcWindow, HDC &_hdcMemory, PAINTSTRUCT & ps);
    virtual void Paint (HWND);
    virtual void Select (bool select){};
    virtual Component * SelectedPort (){ return NULL;};
    void PaintEnd ();    
    void Refresh ();
    virtual bool IsSelected (){return false;};
    virtual void * PortSelected () {return 0;};
    virtual void * GetConnection (int){return 0;};
    void HandleMenu (int command) {};
    virtual void AddMenu () {};
    virtual void HandleMouseDown (HWND hWnd, int _x, int _y);
    virtual void HandleMouseUp (HWND hWnd) {};
    virtual void HandleMouseMove (HWND, int, int);  
	virtual void Reset (){};
	virtual bool IsSet(){return false;};
    virtual void * PinActive (){return 0;};
    virtual void Activate (bool active) {};
    virtual void MoveTo (int, int){};
    void Show (HINSTANCE, HWND, char *);    
    virtual void LoadBMap (char * bmpResource, HBITMAP &hBitMap, BITMAP &bitMap ); 
    virtual void CenterYourself ();
    HBITMAP hbm;
    BITMAP bm;
    HWND   windowHandle;
    int x;
    int y;
    int width;
    int height;
    int xOffset;
    int yOffset;
    HDC hdcWindow;  // Determined in PaintStart based on windowHandle
    HDC hdcMemory;
    PAINTSTRUCT ps;
    bool isActive;
    
  protected:    
    HINSTANCE g_hInst;    
    HWND lastWindow;
};
#endif


