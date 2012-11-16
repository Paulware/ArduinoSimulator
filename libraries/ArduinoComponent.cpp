#include "ArduinoComponent.h"
// 14 Digitals plus 6 analogs can also be used
ArduinoComponent::ArduinoComponent(): Component()
{ 
  gnd = new Pin(this);
  gnd->WriteValue (0,true);
  power = new Pin (this);
  power->WriteValue (1,true);
  for (int i=0; i<MAX_DIGITAL_VALUES; i++)
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
  digitalValues[14] = (digitalInfo) {357,317,false}; 
  digitalValues[15] = (digitalInfo) {373,317,false}; 
  digitalValues[16] = (digitalInfo) {389,317,false}; 
  digitalValues[17] = (digitalInfo) {405,317,false}; 
  digitalValues[18] = (digitalInfo) {421,317,false}; 
  digitalValues[19] = (digitalInfo) {437,317,false}; 
}

ArduinoComponent::~ArduinoComponent()
{
  delete (gnd);
  delete (power);
  for (int i=0; i<MAX_DIGITAL_VALUES; i++)
    delete (d[i]);
}

void ArduinoComponent::Paint(HWND hWnd)
{
  int x,y;
  PAINTSTRUCT ps;
  HDC hdcWindow;
  HDC hdcMemory;
   
  if (hWnd == windowHandle)
  {
   
    Component::Paint (hWnd, hdcWindow, hdcMemory, ps); // Show Arduino image    
   
    // Show image of digitalValues
    for (int i=0; i<MAX_DIGITAL_VALUES; i++)
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
}

void ArduinoComponent::digitalWrite (int pin, int value )
{
  digitalValues[pin].blackRed = (bool) value;
  if ((pin >= 0) && (pin < MAX_DIGITAL_VALUES))
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

