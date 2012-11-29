#include "Component.h"
#include "Menus.h"

Component::Component()
{ 
  g_hInst = NULL; 
  windowHandle = NULL;                  
  hbm = 0;
  x = 0;
  y = 0;
  width = 0;
  height = 0;
  xOffset = 0;
  yOffset = 0;
  hdcWindow = 0;
  hdcMemory = 0;
  bm.bmWidth = 0;
}

void Component::PaintEnd ()
{  
  DeleteDC (hdcMemory);
  EndPaint (windowHandle, &ps); 
}   

void Component::PaintStart (HDC &_hdcWindow, HDC &_hdcMemory, PAINTSTRUCT & _ps)
{
  if (!_hdcWindow)
  {	
    _hdcWindow = BeginPaint(windowHandle, &_ps);
    _hdcMemory = CreateCompatibleDC(_hdcWindow);
  }
  hdcWindow = _hdcWindow;
  hdcMemory = _hdcMemory;
  ps = _ps;
}   

void Component::CenterYourself ( )
{
  x = x + bm.bmWidth/2;
  y = y + bm.bmHeight/2;
}

void Component::HandleMouseDown (HWND hWnd, int _x, int _y)
{
	// This should never be called as the virtual subclass should be called.
}

void Component::Refresh ()
{
  if (windowHandle)
    InvalidateRect ( windowHandle, NULL, true );      
  if (lastWindow)
    InvalidateRect ( lastWindow, NULL, true);  
}

/*
   Input:
       hWnd: handle to current window
       hdcWindow: set by PaintStart above
       hdcMemory: handle to device memory
       
   Note:
       PaintEnd needs to be called after calling this function    
*/
void Component::Paint(HWND hWnd)
{   
  if (hbm)
  {
    SelectObject(hdcMemory, hbm);
    width = bm.bmWidth;
    height = bm.bmHeight;  
    BitBlt(hdcWindow, x, y, bm.bmWidth, bm.bmHeight, hdcMemory, 0, 0, SRCAND);
    BitBlt(hdcWindow, x, y, bm.bmWidth, bm.bmHeight, hdcMemory, 0, 0, SRCPAINT);
  }  
  lastWindow = hWnd;
}

void Component::LoadBMap (char * bmpResource, HBITMAP &hBitMap, BITMAP &bitMap )
{
  if (strlen(bmpResource))
  {   
    hBitMap = LoadBitmap (g_hInst, bmpResource);   
    GetObject(hBitMap, sizeof(bitMap), &bitMap);          
  }       
}

void Component::HandleMouseMove (HWND hWnd, int _x, int _y)
{
  if (bm.bmWidth)
    if ((_x >= x) && (_x <= x+bm.bmWidth) && (_y >= y) && (_y <= y+bm.bmHeight))
      isActive = true;
    else if (isActive)
      isActive = false;    
}

// This function sets bm, and hbm to the specified resource
void Component::Show ( HINSTANCE hInst, HWND hWnd, char * bmpResource)
{
  g_hInst = hInst;
  
  LoadBMap ( bmpResource, hbm, bm ); 
  
  // hbm = LoadBitmap (hInst, bmpResource);   
  GetObject(hbm, sizeof(bm), &bm);          
  windowHandle = hWnd;
  Refresh();   
}

// DrawWindow is only used by the high level windows to draw the actual window with menu
HWND Component::DrawWindow(char * title, HINSTANCE hInst, char * bmpResource, 
                           int x, int y, int width, int height)
{
  g_hInst = hInst; 
  int Style = WS_MINIMIZEBOX | WS_SIZEBOX | WS_CAPTION | WS_MAXIMIZEBOX |
              WS_POPUP | WS_SYSMENU;                    
 
  // Load the bitmap background image
  LoadBMap (bmpResource, hbm, bm);
  
  // Create and show the window
  windowHandle = CreateWindowEx(0,"ListBox1",title,
      Style, x, y, 4+width, 12+height, NULL,(HMENU)NULL,hInst,NULL);      
  SendMessage(windowHandle,(UINT)WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),
    (LPARAM)MAKELPARAM(FALSE,0));
  RedrawWindow(windowHandle,0,0,0);
  ShowWindow(windowHandle,SW_SHOW);
  AddMenu();
  return windowHandle;
}

/*
void Component::AddMenu ()
{
  HMENU  MainMenu;
  HMENU  FileMenu;
     
  //arduinoMain.setHwnd ( hwndOwner);
  MainMenu=CreateMenu();
  FileMenu=CreateMenu();
  InsertMenu(MainMenu,ID_Edit,MF_POPUP,(UINT)FileMenu,"Help");
  AppendMenu(FileMenu,MF_STRING,COMPONENT_ABOUT,"&About");
  AppendMenu(FileMenu,MF_SEPARATOR,0,"");
  AppendMenu(FileMenu,MF_STRING,COMPONENT_HELP,"&Component Help");
  //  activate menu 
  (void) SetMenu(windowHandle,MainMenu);
}


void Component::HandleMenu (int command)
{
  switch (command)
  {
    case COMPONENT_ABOUT:
      MessageBox(windowHandle, "Component About", "TBD", MB_OK | MB_ICONEXCLAMATION);  
    break;
    
    case COMPONENT_HELP:
      MessageBox(windowHandle, "Component Help", "TBD", MB_OK | MB_ICONEXCLAMATION);  
    break;
  }   
}

*/
