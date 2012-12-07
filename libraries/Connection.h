#ifndef CONNECTION_H
#define CONNECTION_H
#include "Component.h"
#include "Pin.h"
#include "Line.h"
class Connection:public Component
{
  public:          
    Connection(Component *, Pin *, Pin *); // Constructor
    ~Connection();       // Destructor
    void LoadBMap (HINSTANCE); 
    void PaintStart (HDC & _hdcWindow ,HDC & _hdcMemory, PAINTSTRUCT &_ps);
    void Paint(HWND hWnd); // line->Paint(hWnd);};                      
    void HandleMouseMove (HWND hWnd, int x1, int y1, int x2, int y2);
    void Move ();
	void HandleMouseDown (HWND hWnd, int _x, int _y){};
    void NotSavedYet(){};
    void SaveConnection ( FILE *);
    Pin * pin1;
    Pin * pin2;
    Component * component1;
	    
  private:
    Line * line;
    int x1;
    int y1;
    int x2;
    int y2;
};
#endif

