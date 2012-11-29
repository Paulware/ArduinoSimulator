#ifndef CONNECTION_H
#define CONNECTION_H
#include "Component.h"
#include "Line.h"
class Connection:public Component
{
  public:          
    Connection(COORD,COORD); // Constructor
    ~Connection();       // Destructor
    void LoadBMap (HINSTANCE); 
    void PaintStart (HDC & _hdcWindow ,HDC & _hdcMemory, PAINTSTRUCT &_ps);
    void Paint(HWND hWnd) {line->Paint(hWnd);};                      
    void HandleMouseMove (HWND hWnd, int x1, int y1, int x2, int y2);
    void MoveTo (int,int,int,int);
	void HandleMouseDown (HWND hWnd, int _x, int _y){};
  private:
    Line * line;
};
#endif


