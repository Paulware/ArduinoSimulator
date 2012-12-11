#ifndef PIN_H
#define PIN_H
#include "Component.h"
//#include "Connection.h"

#define MAX_PIN_CONNECTIONS 10
struct PinValueType
{
  int value;
  int resistance;
};

class Pin
{
  public:          
    // Constructor
    Pin(Component *); 
    ~Pin(); // Clean up the connection
    void SetName(char *);
	int GetValue();
	int GetResistance();
	void Reset();
	bool IsSet();
	void WriteValue (int value, int resistance);
	void WriteValue (int value);
	void Connect ( Pin * pin );
	void LoadBMap ( HINSTANCE);
	void LoadBMap ( HINSTANCE, char *);
    void HandleMouseMove (HWND,int,int);
    void PaintStart (HDC & _hdcWindow, HDC &_hdcMemory, PAINTSTRUCT & ps);
    void Paint(HDC hdcMemory, HDC hdcWindow);
    void Select (bool);
    void MoveTo (int, int);
    static void BestValue (PinValueType &v1, PinValueType &v2);
    
    int x;
    int y;
    int xOffset;
    int yOffset;
    BITMAP bm;
    bool IsSelected();
    bool isSelected;
    char * name;   
    bool isActive;    // Cannot use components because parent has many pins
    Component * parent;
    
    PinValueType value;
    PinValueType constValue;
    

  private:
    HBITMAP hbm;
    HINSTANCE myInst;
};
#endif


