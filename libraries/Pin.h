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

class Pin: public Component
{
  public:          
    // Constructor
    Pin(); 
    ~Pin(); // Clean up the connection
    void SetName(char *);
	int GetValue();
	int GetResistance();
	void Reset();
	bool IsSet();
	void WriteValue (int value, int resistance);
	void WriteValue (int value);
	void Connect ( Pin * pin );
    void HandleMouseMove (HWND,int,int);
    void Init (HWND, HINSTANCE);
    void Paint(HDC _hdc, PAINTSTRUCT _ps, HDC _hdcMemory);
    void Select (bool);
    void MoveTo (int, int);
    static void BestValue (PinValueType &v1, PinValueType &v2);
    int xOffset;
    int yOffset;
    bool IsSelected();
    bool isSelected;
    char * name;   
    bool isActive; 
    
    PinValueType value;
    PinValueType constValue;
    

  private:
    BITMAP redBm;
    HBITMAP hRedBm;
    BITMAP blackBm;
    HBITMAP hBlackBm;
};
#endif


