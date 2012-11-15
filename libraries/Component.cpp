#include "Component.h"

Component::Component()
{ 
  g_hInst = NULL; 
  windowHandle = NULL;   
  bitmapExists = false;                
}

void Component::PaintEnd ( HDC * hdcMemory, HWND hWnd, PAINTSTRUCT * ps)
{
   DeleteDC (*hdcMemory);
   EndPaint (hWnd, ps ); 
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
   Output:
       hdcWindow: handle to device context
       hdcMemory: handle to device memory
       
   Note:
       PaintEnd needs to be called after calling this function    
*/
void Component::Paint(HWND hWnd, HDC &hdcWindow, HDC &hdcMemory, PAINTSTRUCT &ps)
{
       
  hdcWindow = BeginPaint(windowHandle, &ps);
  hdcMemory = CreateCompatibleDC(hdcWindow);
  if (bitmapExists)
  {
    SelectObject(hdcMemory, hbm);
    BitBlt(hdcWindow, 0, 0, bm.bmWidth, bm.bmHeight, hdcMemory, 0, 0, SRCAND);
  }  
  lastWindow = hWnd;
}

void Component::LoadBMap (char * bmpResource, HBITMAP &hBitMap, BITMAP &bitMap )
{
  int len = strlen (bmpResource);
  if (len)
  {   
    hBitMap = LoadBitmap (g_hInst, bmpResource);   
    GetObject(hBitMap, sizeof(bitMap), &bitMap);          
    bitmapExists = true;
  }       
}

HWND Component::DrawWindow(char * title, HINSTANCE hInst, char * bmpResource, 
                           int x, int y, int width, int height)
{
  g_hInst = hInst; 
  int Style = WS_MINIMIZEBOX | WS_SIZEBOX | WS_CAPTION | WS_MAXIMIZEBOX |
              WS_POPUP | WS_SYSMENU;                    
    
  // Load the bitmap background image
  LoadBMap ( bmpResource, hbm, bm );

  // Create and show the window
  windowHandle = CreateWindowEx(0,"ListBox1",title,
      Style, x, y, 4+width, 12+height, NULL,(HMENU)NULL,hInst,NULL);      
  SendMessage(windowHandle,(UINT)WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),
    (LPARAM)MAKELPARAM(FALSE,0));
  RedrawWindow(windowHandle,0,0,0);
  ShowWindow(windowHandle,SW_SHOW);
  return windowHandle;
}
