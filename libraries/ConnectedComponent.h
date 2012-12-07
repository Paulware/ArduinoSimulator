#ifndef CONNECTEDCOMPONENT_H
#define CONNECTEDCOMPONENT_H
#include "Component.h"
#include "Connection.h"
#include "Pin.h"
/* 
  Connected Commponents are Components which also can be connected to other components
  As opposed to simple components like Dots which can only be displayed
*/
#define CONNECTIONSPERCOMPONENT 20
class ConnectedComponent:public Component
{
  public:          
    ConnectedComponent(int,int); 
    ~ConnectedComponent(); 
    void Connect (Pin *, Pin *,HINSTANCE);
    void SaveConnections(FILE * fp);
    void PaintStart (HDC & _hdcWindow, HDC &_hdcMemory, PAINTSTRUCT & ps);
    void Paint (HWND);
    void Move ();
    Pin * pin1;
    Pin * pin2;
    virtual void SetPins();	
	// Create setter/accessor for next 2 items?		    
    int numConnections;
	Connection * connections[CONNECTIONSPERCOMPONENT];	
};
#endif


