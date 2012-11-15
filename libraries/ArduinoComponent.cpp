#include "ArduinoComponent.h"
// 14 Digitals plus 6 analogs can also be used
#define MAX_DIGITALS 20

ArduinoComponent::ArduinoComponent(): Component()
{ 
  gnd = new Pin(this);
  gnd->WriteValue (0,true);
  for (int i=0; i<MAX_DIGITALS; i++)
  {
    d[i] = new Pin(this);  
    d[i]->WriteValue (0,true );
  }  
  
  digitalValues[0]  = (digitalInfo) {438,8,false};
  digitalValues[1]  = (digitalInfo) {422,8,false};
  digitalValues[2]  = (digitalInfo) {406,8,false};
  digitalValues[3]  = (digitalInfo) {390,8,false};
  digitalValues[4]  = (digitalInfo) {374,8,false};
  digitalValues[5]  = (digitalInfo) {358,8,false};
  digitalValues[6]  = (digitalInfo) {342,8,false};
  digitalValues[7]  = (digitalInfo) {324,8,false};
  digitalValues[8]  = (digitalInfo) {298,8,false};
  digitalValues[9]  = (digitalInfo) {282,8,false};
  digitalValues[10] = (digitalInfo) {266,8,false};
  digitalValues[11] = (digitalInfo) {250,8,false};
  digitalValues[12] = (digitalInfo) {234,8,false};
  digitalValues[13] = (digitalInfo) {218,8,false};
  strcpy(BCX_ClassName,"ListBox1");
}

ArduinoComponent::~ArduinoComponent()
{
  if (gnd)
    delete (gnd);
  for (int i=0; i<MAX_DIGITALS; i++)
    delete (d[i]);
}

void ArduinoComponent::Paint(HWND hWnd)
{
   int x,y;
   PAINTSTRUCT ps;
   HDC hdcWindow;
   HDC hdcMemory;
   
   Component::Paint (hWnd, hdcWindow, hdcMemory, ps); // Show Arduino image    
   
   // Show image of digitalValues
   for (int i=0; i<14; i++)
   {
     x = digitalValues[i].x;
     y = digitalValues[i].y;
     if (digitalValues[i].blackRed)  
       SelectObject(hdcMemory, hbmRedDot);
     else
       SelectObject(hdcMemory, hbmBlackDot);
     BitBlt(hdcWindow, x,y, bmRedDot.bmWidth, bmRedDot.bmHeight, hdcMemory, 0, 0, SRCAND);
     BitBlt(hdcWindow, x,y, bmRedDot.bmWidth, bmRedDot.bmHeight, hdcMemory, 0, 0, SRCPAINT);
   }
   
   Component::PaintEnd ( &hdcMemory, hWnd, &ps);  
}

void ArduinoComponent::digitalWrite (int pin, int value )
{
  digitalValues[pin].blackRed = (bool) value;
  if ((pin >= 0) && (pin < MAX_DIGITALS))
    d[pin]->WriteValue (value,true);
    
  Component::Refresh();
}

HWND ArduinoComponent::DrawWindow(char * title, HINSTANCE hInst, 
                                  int x, int y)
{                               
   HWND hWnd = Component::DrawWindow(title, hInst, "ARDUINOBMP", x, y, 600, 400);

   // Load additional bitmaps   
   Component::LoadBMap ("REDDOT", hbmRedDot, bmRedDot);
   Component::LoadBMap ("BLACKDOT", hbmBlackDot, bmBlackDot); 
   return hWnd;
}

