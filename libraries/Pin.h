#ifndef PIN_H
#define PIN_H
#include "Component.h"
//#include "Connection.h"

#define MAX_PIN_CONNECTIONS 10
class Pin
{
  public:          
    // Constructor
    Pin(Component *); 
    ~Pin(); // Clean up the connection
    void SetName(char *);
	int GetValue();
	void Reset();
	bool IsSet();
	void WriteValue (int);
	void Connect ( Pin * pin );
	void LoadBMap ( HINSTANCE);
	void LoadBMap ( HINSTANCE, char *);
    void HandleMouseMove (HWND,int,int);
    void PaintStart (HDC & _hdcWindow, HDC &_hdcMemory, PAINTSTRUCT & ps);
    void Paint(HDC hdcMemory, HDC hdcWindow);
    void Select (bool);
    void MoveTo (int, int);
    int x;
    int y;
    int xOffset;
    int yOffset;
    BITMAP bm;
    bool IsSelected();
    bool isSelected;
    char * name;   
    int constValue;
    bool isActive;    // Cannot use components because parent has many pins
    Component * parent;

  private:
    int value;
    HBITMAP hbm;
    HINSTANCE myInst;
};
#endif


