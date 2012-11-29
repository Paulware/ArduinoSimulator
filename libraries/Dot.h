#ifndef DOT_H
#define DOT_H
#include "Component.h"
class Dot:public Component
{
  public:          
    Dot(int,int); // Constructor
    ~Dot(); // Destructor
    void LoadBMap (HINSTANCE);
    void PaintStart (HDC & _hdcWindow ,HDC & _hdcMemory, PAINTSTRUCT &_ps);
    void Paint(HWND hWnd);                      
    void HandleMouseMove (HWND hWnd, int _x, int _y);  
    void HandleMouseDown (HWND hWnd, int _x, int _y);  
    void MoveTo (int, int);  
  private:
};
#endif


