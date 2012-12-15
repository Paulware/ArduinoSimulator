#ifndef HIGHLEVELMENU_H
#define HIGHLEVELMENU_H
#include "ConnectedComponent.h"
#include "ViewConnections.h"
#include <vector>
#include <string>

#define MAX_COMPONENTS 100
class HighLevelMenu:public Component
{
  public:
    HighLevelMenu(ViewConnections * _viewConnections); 
    ~HighLevelMenu();
    static HighLevelMenu * Instance();    
    void ComputeSystem();
    HWND DrawWindow (char * title, HINSTANCE hInst, char * bmpResource, 
                     int x , int y, int width=0, int height=0);  
    void PaintStart ();
    void Paint(HWND hWnd);
    void HandleMouseMove (HWND hWnd, int x, int y);
    void HandleMenu (int command); 
    void AddMenu ();
    void AddComponent (int index, char * typeName, int x, int y);
    void HandleMouseDown (HWND hWnd, int x, int y);
    void HandleMouseUp (HWND hWnd);
    void HandleKeyUp ( int scanCode); 
    void DeleteConnection (Pin * pin);
	void NewProject();
    void SaveProject(char *);
    void NotSavedYet ();    
    void ReadProject(char *);
    int DigitalRead(int pin);
    void DigitalWrite (int,int);
    ConnectedComponent * FindComponent (int,int );
    ConnectedComponent * FindComponent ( char * typeName);
    int FindComponentName (std::string str);
 
    ConnectedComponent * components[MAX_COMPONENTS];   
    void lcdPrint ( char * ch);
    void lcdPrint ( int value);   
    void lcdClear ();
    void Reset ();
    Pin * FindOtherPin ( Pin *);
    void TroubleshootPins ();
            
  private:
    int numComponents;
    int selectedItem;      
    ViewConnections * viewConnections;
    char projectFilename[80];
    std::vector<std::string> ComponentNames;
};

#endif


