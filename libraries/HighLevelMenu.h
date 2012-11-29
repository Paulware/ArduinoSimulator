#ifndef HIGHLEVELMENU_H
#define HIGHLEVELMENU_H
#include "Component.h"
#include "Connection.h"
#include "ViewConnections.h"

#define MAX_COMPONENTS 100
class HighLevelMenu:public Component
{
  public:
    HighLevelMenu(); 
    ~HighLevelMenu();
    void ComputeSystem();
    HWND DrawWindow (char * title, HINSTANCE hInst, char * bmpResource, 
                     int x , int y, int width=0, int height=0);  
    void PaintStart ();
    void Paint(HWND hWnd);
    void HandleMouseMove (HWND hWnd, int x, int y);
    void HandleMenu (int command); 
    void AddMenu ();
    void HandleMouseDown (HWND hWnd, int x, int y);
    void HandleMouseUp (HWND hWnd);
    
    Component * components[MAX_COMPONENTS];   
    Connection * connections[MAX_COMPONENTS];
  private:
    int numComponents;
	int numConnections; 
    int selectedItem;      
    ViewConnections * viewConnections;
};

#endif


