#include "SevenSeg.h"
SevenSeg::SevenSeg(int _x, int _y):Component()
{ 
  int xs[] = {25,50,75,5,25,50,75};                       
  int ys[] = {135,135,135,70,10,10,10};
  x = _x;
  y = _y;
  gnd = new Pin(this);
  gnd->xOffset = 95;
  gnd->yOffset = 70;
  gnd->x = x + gnd->xOffset;
  gnd->y = y + gnd->yOffset;
  
  for (int i=0; i<7;i++)
  {
    segment[i] = new Pin(this);
	segment[i]->WriteValue (0);
	segment[i]->xOffset = xs[i];
	segment[i]->yOffset = ys[i];
	segment[i]->x = x + segment[i]->xOffset;
	segment[i]->y = y + segment[i]->yOffset;
  }	
  segmentValues[5] = (SegmentInfo) { 30,   7,  true};
  segmentValues[4] = (SegmentInfo) { 30,  55,  true};
  segmentValues[1] = (SegmentInfo) { 80,   7,  true};
  segmentValues[2] = (SegmentInfo) { 80,  55,  true};
  segmentValues[0] = (SegmentInfo) { 35,   0, false};
  segmentValues[6] = (SegmentInfo) { 35,  50, false};
  segmentValues[3] = (SegmentInfo) { 35,  98, false};
}

// [_x,_y] are absolute values
void SevenSeg::MoveTo (int _x, int _y)
{
  x = _x-xOffset;
  y = _y-yOffset; // Get the x location of the LED after adjusting for mouse click location
  gnd->MoveTo (x + gnd->xOffset - gnd->bm.bmWidth/2,
               y + gnd->yOffset - gnd->bm.bmHeight/2);    
  for (int i=0; i<7; i++)
  	segment[i]->MoveTo (x + segment[i]->xOffset - segment[i]->bm.bmWidth/2,
                        y + segment[i]->yOffset - segment[i]->bm.bmHeight/2);
}

void SevenSeg::HandleMouseDown (HWND hWnd, int _x, int _y)
{
  Component::HandleMouseDown (hWnd,_x,_y);
  if (gnd->isActive) // we are over the ground spot     
    gnd->Select(!gnd->isSelected);
  else
  {
    for (int i=0; i<7; i++)
    {
      if (segment[i]->isActive)
      {  
        segment[i]->Select(!segment[i]->isSelected);
        break; 
      }   
    }
  } 
}


Pin * SevenSeg::PinActive ()
{
  Pin * pin = 0;
  if (gnd->isActive)
    pin = gnd;
  else 
    for (int i=0; i<7;i++)
    {
      if (segment[i]->isActive)
      {
        pin = segment[i];
        break;
      }
    }  
  
  return pin;
}

SevenSeg::~SevenSeg()
{
  delete (gnd);                    
  for (int i=0; i<7; i++)
    delete (segment[i]);
}

// Check if the mouse has moved over one of the ports.
void SevenSeg::HandleMouseMove (HWND hWnd, int _x, int _y)
{
  gnd->HandleMouseMove ( hWnd, _x, _y );
  for (int i=0; i<7; i++)
    segment[i]->HandleMouseMove (hWnd, _x, _y);
}

void SevenSeg::AddMenu ()
{
  HMENU  MainMenu;
  HMENU  FileMenu;
     
  //arduinoMain.setHwnd ( hwndOwner);
  MainMenu=CreateMenu();
  FileMenu=CreateMenu();
  InsertMenu(MainMenu,ID_Edit,MF_POPUP,(UINT_PTR)FileMenu,"Help");
  AppendMenu(FileMenu,MF_STRING,SEVENSEGMENT_ABOUT,"&About");
  AppendMenu(FileMenu,MF_SEPARATOR,0,"");
  AppendMenu(FileMenu,MF_STRING,SEVENSEGMENT_HELP,"&Component Help");
  //  activate menu 
  (void) SetMenu(windowHandle,MainMenu);
}

void SevenSeg::HandleMenu ( int command )
{
  switch (command)
  {
    case SEVENSEGMENT_ABOUT:
      MessageBox(windowHandle, "7 Segment About", "TBD", MB_OK | MB_ICONEXCLAMATION);  
    break;
    
    case SEVENSEGMENT_HELP:
      MessageBox(windowHandle, "7 Segment Help", "TBD", MB_OK | MB_ICONEXCLAMATION);  
    break;
  }       
}

void SevenSeg::Paint(HWND hWnd)
{
  int x = 31;
  int y = 0;
  int width;
  int height;
  bool vertical;
  bool offOn;
   
  if (hWnd == windowHandle)
  {
    Component::Paint (hWnd); // Show Arduino image    
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
      segment[i]->Paint (hdcMemory,hdcWindow);        
    }
    gnd->Paint(hdcMemory,hdcWindow);
    
  }    
}

void SevenSeg::LoadBMap (char * bmpResource, HBITMAP &hBitMap, BITMAP &bitMap )
{
   Component::LoadBMap (bmpResource, hBitMap, bitMap );     
   Component::LoadBMap ("LEFTRIGHT", hbmLeftRight, bmLeftRight);
   Component::LoadBMap ("UPDOWN", hbmUpDown, bmUpDown); 
   
   gnd->LoadBMap (g_hInst);
   for (int i=0; i<7;i++)
    segment[i]->LoadBMap (g_hInst);
}

HWND SevenSeg::DrawWindow (char * title, HINSTANCE hInst, char * bmpResource, 
                           int x , int y, int width, int height)
{                           
   HWND hWnd = Component::DrawWindow(title, hInst, "SEVENSEGMENT", x, y, 100, 160);  
}

Pin * SevenSeg::PortSelected(){
  Pin * pin = 0;
  if (gnd->isSelected)
  	pin = gnd;
  if (!pin)
    for (int i=0; i<7; i++)
	  if (segment[i]->isSelected)
	  {
	  	pin = segment[i];
	  	break;
	  }	
	  
  return pin;  
}

void SevenSeg::PaintStart ( HDC & _hdcWindow, HDC & _hdcMemory, PAINTSTRUCT &_ps)
{
  Component::PaintStart ( _hdcWindow, _hdcMemory, _ps);
  
  gnd->PaintStart ( _hdcWindow, _hdcMemory, _ps);
  for (int i=0; i<7; i++)
    segment[i]->PaintStart ( _hdcWindow, _hdcMemory, _ps);
}


