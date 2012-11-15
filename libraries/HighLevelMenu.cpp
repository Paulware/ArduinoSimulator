#include "HighLevelMenu.h"
#include "windows.h"
#define ID_Exit 101
#define ID_Edit 201
#define ID_Load 301
#define ID_Save 302
#define ID_Run  303
HighLevelMenu::HighLevelMenu():Component()
{ 
}

BOOL HighLevelMenu::AddMenu (HWND hwndOwner)
{
  HMENU  MainMenu;             // has to be HMENU type
  HMENU  FileMenu;             // dito
     
  //arduinoMain.setHwnd ( hwndOwner);
  MainMenu=CreateMenu();
  FileMenu=CreateMenu();
  InsertMenu(MainMenu,ID_Edit,MF_POPUP,(UINT)FileMenu,"File");
  AppendMenu(FileMenu,MF_STRING,ID_Load,"&Load");
  AppendMenu(FileMenu,MF_STRING,ID_Save,"&Save");
  AppendMenu(FileMenu,MF_STRING,ID_Run, "&Run");
  AppendMenu(FileMenu,MF_SEPARATOR,0,"");
  AppendMenu(FileMenu,MF_STRING,ID_Exit,"E&xit");
  //  activate menu 
  if (!SetMenu(hwndOwner,MainMenu)) {
    return FALSE;
  }
  return TRUE;
}

void HighLevelMenu::Paint(HWND hWnd)
{
  PAINTSTRUCT ps;
  HDC hdcWindow;
  HDC hdcMemory;
   
  Component::Paint (hWnd, hdcWindow, hdcMemory, ps); // Show Arduino image    
  Component::PaintEnd ( &hdcMemory, hWnd, &ps);
}

HWND HighLevelMenu::DrawWindow(char * title, HINSTANCE hInst, int x, int y)
{                            
  HWND hWnd = Component::DrawWindow(title, hInst, "", x, y, 200,100);
  AddMenu(windowHandle);
  return hWnd;
}

