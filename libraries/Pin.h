#ifndef PIN_H
#define PIN_H
#include "Component.h"
class Pin
{
  public:          
    // Constructor
    Pin(Component *); 
    
	int GetValue();
	void WriteValue (int, bool);
	void Connect ( Pin * pin );

  private:
    int value;
    Pin * connectedTo;
    Component * parent;
};

#endif


