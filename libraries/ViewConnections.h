#ifndef VIEWCONNECTIONS_H
#define VIEWCONNECTIONS_H
#include "Component.h"

class ViewConnections:public Component
{
  public:	
    ViewConnections(int,int); 
    ~ViewConnections();
    void SetConnection (int i, char * value);
    HWND DrawWindow (HINSTANCE); 
    void PaintStart ();
    void Paint(HWND hWnd);
    void HandleMouseMove (HWND hWnd, int x, int y){};
    void HandleMenu (int command){}; 
    void AddMenu (){};
    void AddDevice ( char * name, Component * component);
    void HandleMouseDown (HWND hWnd, int x, int y){};
    void HandleMouseUp (HWND hWnd){};
    
  private:
    static int const numConnections = 22;
	char * connections[numConnections];	
	struct DeviceType 
	{
	  char * name;
	  Component * component;	
	};  
	static int const maxDevices = 100;
	DeviceType devices[maxDevices];	
	int numDevices;
};
#endif


