#ifndef LINE_H
#define LINE_H
#include "Component.h"
#include "Dot.h"
#define DOTSPERLINE 40
class Line:public Component
{
  public:          
    Line(int,int,int,int); // Constructor
    ~Line(); // Destructor
    void PaintStart ( HDC & _hdcWindow, HDC & _hdcMemory, PAINTSTRUCT &_ps);
    void Paint(HWND hWnd);                      
    void HandleMouseMove (HWND hWnd, int _x, int _y){};  
    void HandleMouseDown (HWND hWnd, int _x, int _y);  
    void MoveTo (int, int, int, int);  
    void LoadBMap (HINSTANCE);
	 
  private:
    HBITMAP       hbmRedDot;
    BITMAP        bmRedDot;

    HBITMAP       hbmBlackDot;
    BITMAP        bmBlackDot;
    
    int x1;
    int y1;
    int x2; 
    int y2;
    
    Dot * dot[DOTSPERLINE];
};
#endif


