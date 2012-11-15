#ifndef HIGHLEVELMENU_H
#define HIGHLEVELMENU_H
#include "Component.h"
class HighLevelMenu:Component
{
  public:          
    // Constructor
    HighLevelMenu(); 
    HWND DrawWindow(char * title, HINSTANCE hInst, int x, int y);
    void Paint(HWND hWnd);
    
    BOOL AddMenu (HWND hwndOwner);
 
  private:
          
};

#endif


