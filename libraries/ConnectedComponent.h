#ifndef CONNECTEDCOMPONENT_H
#define CONNECTEDCOMPONENT_H
#include "Component.h"
#include "Connection.h"
#include "Pin.h"
/* 
  Connected Commponents are Components which also can be connected to other components
  As opposed to simple components like Dots which can only be displayed
*/

class ConnectedComponent:public Component
{
  public:          
    ConnectedComponent(int,int); 
    ~ConnectedComponent(); 
    // Delete a connection is the specified pin matches one of the component's pins
    void DeleteConnection (Pin * pin);
    void Connect (Pin * pin1, Pin * pin2);
    void SaveConnections(FILE * fp);
    void Paint (HDC _hdc, PAINTSTRUCT _ps, HDC _hdcMemory);
    void Move ();
    Pin * pin1;
    Pin * pin2;
    virtual void SetPins();	
    virtual void Select (bool) = 0;    
	// Create setter/accessor for next 2 items?		    
    int numConnections;
    static int const CONNECTIONSPERCOMPONENT = 20;    
	Connection * connections[CONNECTIONSPERCOMPONENT];	
    Pin * FindOtherPin (Pin *);    	
    virtual bool TroubleshootPins (){return true;};
	virtual void Init (HWND _windowHandle, HINSTANCE _g_hInst, char * resource);  
	virtual void CleanUp(); 
};
#endif


