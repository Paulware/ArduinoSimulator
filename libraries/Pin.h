#ifndef PIN_H
#define PIN_H
#include "Component.h"
#define MAX_PIN_CONNECTIONS 10
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
    Pin * connectedTo[MAX_PIN_CONNECTIONS];
    Component * parent;
    int numConnections;
};

#endif


