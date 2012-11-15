#include "KeyboardMonitor.h"
#include "windows.h"
#include "SerialClass.h"
KeyboardMonitor::KeyboardMonitor():Component()
{ 
  hWndEdit = NULL;
}


void KeyboardMonitor::Paint(HWND hWnd)
{
  PAINTSTRUCT ps;
  HDC hdcWindow;
  HDC hdcMemory;
   
  Component::Paint (hWnd, hdcWindow, hdcMemory, ps); // Show Arduino image    
  Component::PaintEnd ( &hdcMemory, hWnd, &ps);
}

HWND KeyboardMonitor::DrawWindow(char * title, HINSTANCE hInst, int x, int y)
{                           
  HWND textHandle;
  HWND hWnd;
  hWnd = Component::DrawWindow(title, hInst, "", x, y, 450,550);
  int Style = WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP;
  int Exstyle = WS_EX_CLIENTEDGE; 
  int id = 1009;
  
  // Create the edit box 
  hWndEdit = CreateWindow ( TEXT ( "edit"), TEXT ( ""), 
          WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL,  
          10,10,300,20,
          windowHandle, (HMENU)11, NULL, NULL );   
          
  
  // Create the list box 
  textHandle = CreateWindowEx(WS_EX_CLIENTEDGE,"Listbox",NULL,
               WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP,
               10, 40, 400, 450, windowHandle, (HMENU)id, hInst, NULL);
  SendMessage(textHandle,(UINT)WM_SETFONT,(WPARAM)GetStockObject
             (DEFAULT_GUI_FONT),(LPARAM)MAKELPARAM(FALSE,0));
  Serial.setTWindow (textHandle); // Set text window where Serial will write to?
  
  return hWnd;
}

void KeyboardMonitor::GetWindowTxt ( TCHAR * szText )
{
  int len;
  if (hWndEdit)
  {
    GetWindowText ( hWndEdit, szText, 511 );
    len = strlen (szText);
    for (int i=0; i<len; i++)
    {
      if ((char) *(szText+i) == '\r')
      {
        SetWindowText ( hWndEdit, "" ); 
        break;
      }      
    }  
  }    
}


