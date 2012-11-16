#include "MomentaryDepress.h"
MomentaryDepress::MomentaryDepress():Component()
{ 
  depressed = false;
  input = new Pin(this);
  output = new Pin(this);
}

MomentaryDepress::~MomentaryDepress()
{
  delete (input);
  delete (output);
}

void MomentaryDepress::HandleMouseDown (HWND hWnd)
{
  if (hWnd == windowHandle)
  {
    depressed = true;
    output->WriteValue ( input->GetValue(), true);
    Refresh();  
  }
}

void MomentaryDepress::HandleMouseUp (HWND hWnd)
{
  if (hWnd == windowHandle)
  {
    depressed = false;
    output->WriteValue ( -1, true);
    Refresh();  
  }
}


void MomentaryDepress::Paint(HWND hWnd)
{
  PAINTSTRUCT ps;
  HDC hdcWindow;
  HDC hdcMemory;
  bool connected; 
  int x = 40;
  int y = 17;
   
  if (hWnd == windowHandle)
  {
    Component::Paint (hWnd, hdcWindow, hdcMemory, ps); // Show Arduino image    

    if (depressed)
    {
      SelectObject(hdcMemory, hbmDepressed);
     
      BitBlt(hdcWindow, x,y, bmDepressed.bmWidth, bmDepressed.bmHeight, hdcMemory, 0, 0, SRCAND);
      BitBlt(hdcWindow, x,y, bmDepressed.bmWidth, bmDepressed.bmHeight, hdcMemory, 0, 0, SRCPAINT);     
    } 
    Component::PaintEnd ( &hdcMemory, hWnd, &ps);  
  }  
}

HWND MomentaryDepress::DrawWindow(char * title, HINSTANCE hInst,  
                                  int x, int y)
{                           
   HWND hWnd = Component::DrawWindow(title, hInst, "MOMENTARYDEPRESS", x, y, 100, 120);

   // Load additional bitmaps   
   Component::LoadBMap ("DEPRESSED", hbmDepressed, bmDepressed);
}

