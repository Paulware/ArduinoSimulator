#include "Led.h"
Led::Led():Component()
{ 
  offOn = false;
  gnd = new Pin(this);
  power = new Pin(this);
  gnd->WriteValue (0,false);
  power->WriteValue (0,false);
}

Led::~Led()
{
  if (gnd)
    delete (gnd);
  if (power)  
    delete (power);
}

void Led::Paint(HWND hWnd)
{
   PAINTSTRUCT ps;
   HDC hdcWindow;
   HDC hdcMemory;
   bool ledOn; 
   int x = 31;
   int y = 0;
   
   Component::Paint (hWnd, hdcWindow, hdcMemory, ps); // Show Arduino image    

   if (gnd->GetValue() || !power->GetValue())
     ledOn = false;
   else
     ledOn = true;

   if (ledOn)
     SelectObject(hdcMemory, hbmRedDot);
   else
     SelectObject(hdcMemory, hbmBlackDot);
     
   BitBlt(hdcWindow, x,y, bmRedDot.bmWidth, bmRedDot.bmHeight, hdcMemory, 0, 0, SRCAND);
   BitBlt(hdcWindow, x,y, bmRedDot.bmWidth, bmRedDot.bmHeight, hdcMemory, 0, 0, SRCPAINT);

   Component::PaintEnd ( &hdcMemory, hWnd, &ps);  
}

HWND Led::DrawWindow(char * title, HINSTANCE hInst,  
                     int x, int y)
{                           
   HWND hWnd = Component::DrawWindow(title, hInst, "LEDBMP", x, y, 170, 220);

   // Load additional bitmaps   
   Component::LoadBMap ("REDLED", hbmRedDot, bmRedDot);
   Component::LoadBMap ("BLACKLED", hbmBlackDot, bmBlackDot); 
}

