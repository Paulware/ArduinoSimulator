#include "LCDDisplay.h"

LCDDisplay::LCDDisplay():Component()
{ 
  offOn = false;
  gnd = new Pin(this);
  power = new Pin(this);
  gnd->WriteValue (0,false);
  power->WriteValue (0,false);
}

LCDDisplay::~LCDDisplay()
{
  if (gnd)
    delete (gnd);
  if (power)  
    delete (power);
}

void LCDDisplay::Paint(HWND hWnd)
{
  PAINTSTRUCT ps;
  HDC hdcWindow;
  HDC hdcMemory;
  bool ledOn; 
  int x = 31;
  int y = 0;
  LPCTSTR lpString = "Score:";
  HFONT hFont;
   
  if (hWnd == windowHandle)
  {
    Component::Paint (hWnd, hdcWindow, hdcMemory, ps); // Show Arduino image    
    hFont = CreateFont (28,0,0,0,FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
    CLIP_DEFAULT_PRECIS, 0, VARIABLE_PITCH, TEXT ( "Courier New"));
    (void) SetTextColor ( hdcWindow, RGB (255,255,255));
    (void) SetBkMode( hdcWindow, TRANSPARENT);
    SelectObject (hdcWindow, hFont);
    
    TextOut (hdcWindow, 55,  73,  "12345678901234567890", 20 );
    TextOut (hdcWindow, 55, 103,  " Hello Howdy    Bye ", 20 );
    TextOut (hdcWindow, 55, 133,  "I am doing Great !!!", 20 );
    TextOut (hdcWindow, 55, 163,  "That is all bye!    ", 20 );

    /*
    ledOn = false;
    if ((gnd->GetValue() == 0) && (power->GetValue() == 1))
      ledOn = true;
      
    if (ledOn)
      SelectObject(hdcMemory, hbmRedDot);
    else
      SelectObject(hdcMemory, hbmBlackDot);
     
    BitBlt(hdcWindow, x,y, bmRedDot.bmWidth, bmRedDot.bmHeight, hdcMemory, 0, 0, SRCAND);
    BitBlt(hdcWindow, x,y, bmRedDot.bmWidth, bmRedDot.bmHeight, hdcMemory, 0, 0, SRCPAINT);
    */

    Component::PaintEnd ( &hdcMemory, hWnd, &ps);
  }    
}

HWND LCDDisplay::DrawWindow(char * title, HINSTANCE hInst,  
                     int x, int y)
{                           
   HWND hWnd = Component::DrawWindow(title, hInst, "LCDDISPLAY", x, y, 400, 300);

   // Load additional bitmaps   
   //Component::LoadBMap ("REDLED", hbmRedDot, bmRedDot);
   //Component::LoadBMap ("BLACKLED", hbmBlackDot, bmBlackDot); 
}

