#include "SevenSeg.h"
SevenSeg::SevenSeg():Component()
{ 
  gnd = new Pin(this);
  for (int i=0; i<7;i++)
  {
    segment[i] = new Pin(this);
	segment[i]->WriteValue (0,false);
  }	
  segmentValues[5]  = (SegmentInfo) { 30,   7,  true};
  segmentValues[4]  = (SegmentInfo) { 30,  55,  true};
  segmentValues[1]  = (SegmentInfo) { 80,   7,  true};
  segmentValues[2]  = (SegmentInfo) { 80,  55,  true};
  segmentValues[0]  = (SegmentInfo) { 35,   0, false};
  segmentValues[6]  = (SegmentInfo) { 35,  50, false};
  segmentValues[3]  = (SegmentInfo) { 35,  98, false};
}

SevenSeg::~SevenSeg()
{
  for (int i=0; i<7; i++)
    delete (segment[i]);
}

void SevenSeg::Paint(HWND hWnd)
{
  PAINTSTRUCT ps;
  HDC hdcWindow;
  HDC hdcMemory; 
  int x = 31;
  int y = 0;
  int width;
  int height;
  bool vertical;
  bool offOn;
   
  if (hWnd == windowHandle)
  {
    Component::Paint (hWnd, hdcWindow, hdcMemory, ps); // Show Arduino image    
    for (int i=0; i<7; i++)
    {  
      x = segmentValues[i].x;
      y = segmentValues[i].y;
      vertical = segmentValues[i].horizontalVertical;
      offOn = false;
      if ((gnd->GetValue() == 0) && (segment[i]->GetValue() == 1))
        offOn = true;
      if (offOn)  
      {
        if (vertical)                          
        { 
          SelectObject(hdcMemory, hbmUpDown);
          width = bmUpDown.bmWidth;
          height = bmUpDown.bmHeight;
        }  
        else
        { 
          SelectObject(hdcMemory, hbmLeftRight);
          width = bmLeftRight.bmWidth;
          height = bmLeftRight.bmHeight;
        }  
        BitBlt(hdcWindow, x,y, width, height, hdcMemory, 0, 0, SRCAND);
        BitBlt(hdcWindow, x,y, width, height, hdcMemory, 0, 0, SRCPAINT);
      }        
    }
    Component::PaintEnd ( &hdcMemory, hWnd, &ps);
  }    
}

HWND SevenSeg::DrawWindow(char * title, HINSTANCE hInst, int x, int y)
{                           
   HWND hWnd = Component::DrawWindow(title, hInst, "SEVENSEGMENT", x, y, 100, 135);

   // Load additional bitmaps   
   
   Component::LoadBMap ("LEFTRIGHT", hbmLeftRight, bmLeftRight);
   Component::LoadBMap ("UPDOWN", hbmUpDown, bmUpDown); 
   
}

