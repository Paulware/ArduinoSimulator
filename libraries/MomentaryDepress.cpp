#include "MomentaryDepress.h"
MomentaryDepress::MomentaryDepress(int _x, int _y):Component()
{ 
  depressed = false;
  input = new Pin(this);
  output = new Pin(this);
  input->xOffset = 31;
  input->yOffset = 87;
  output->xOffset = 82;
  output->yOffset = 71; 
  x = _x;
  y = _y;
  input->x = x + input->xOffset;
  input->y = y + input->yOffset;    
  output->x = x + output->xOffset;
  output->y = y + output->yOffset; 
  input->SetName ("Switch Left");
  output->SetName ("Switch Right");       
}

MomentaryDepress::~MomentaryDepress()
{
  delete (input);
  delete (output);
}

Pin * MomentaryDepress::PinActive ()
{
  Pin * pin = 0;
  if (input->isActive)
    pin = input;
  else if (output->isActive)
    pin = output;
  return pin;
}

// Check if the mouse has moved over one of the ports.
void MomentaryDepress::HandleMouseMove (HWND hWnd, int _x, int _y)
{
  Component::HandleMouseMove (hWnd,_x,_y);
  input->HandleMouseMove  (hWnd, _x, _y );
  output->HandleMouseMove (hWnd, _x, _y );  
}


void MomentaryDepress::HandleMouseDown (HWND hWnd, int _x, int _y)
{
  if (input->isActive) // we are over the ground spot     
    input->Select(!input->isSelected);
  else if (output->isActive)
    output->Select(!output->isSelected);  
  else
  {
    depressed = true;
    if (output->GetValue() != -1)
      input->WriteValue ( output->GetValue());
    else if (input->GetValue () != -1)
	  output->WriteValue ( input->GetValue());  
  }  
}

bool MomentaryDepress::IsSet()
{
  bool set = input->IsSet () && output->IsSet();
  return set;
}

void MomentaryDepress::Reset ()
{
  input->Reset();
  output->Reset();
}

void MomentaryDepress::HandleMouseUp (HWND hWnd)
{
  if ((hWnd == windowHandle) && !PinActive())
  {
    depressed = false;
    Reset();
  }
}

void MomentaryDepress::AddMenu ()
{
  HMENU  MainMenu;
  HMENU  FileMenu;
     
  //arduinoMain.setHwnd ( hwndOwner);
  MainMenu=CreateMenu();
  FileMenu=CreateMenu();
  InsertMenu(MainMenu,ID_Edit,MF_POPUP,(UINT_PTR)FileMenu,"Help");
  AppendMenu(FileMenu,MF_STRING,DEPRESSSWITCH_ABOUT,"&About");
  AppendMenu(FileMenu,MF_SEPARATOR,0,"");
  AppendMenu(FileMenu,MF_STRING,DEPRESSSWITCH_WIRE,"&How to Wire");
  //  activate menu 
  (void) SetMenu(windowHandle,MainMenu);
}

void MomentaryDepress::Paint(HWND hWnd)
{
  bool connected; 
  int buttonX = 29;
  int buttonY = 10;
    
  if (hWnd == windowHandle)
  {
    Component::Paint (hWnd); // Show Arduino image    

    if (depressed)
    {
      SelectObject(hdcMemory, hbmDepressed);
     
      BitBlt(hdcWindow, x+buttonX,y+buttonY, bmDepressed.bmWidth, bmDepressed.bmHeight, hdcMemory, 0, 0, SRCAND);
      BitBlt(hdcWindow, x+buttonX,y+buttonY, bmDepressed.bmWidth, bmDepressed.bmHeight, hdcMemory, 0, 0, SRCPAINT);     
    } 
    
    // Paint the hotspot
    input->Paint(hdcMemory,hdcWindow);
    output->Paint(hdcMemory,hdcWindow);
    
  }  
}

void MomentaryDepress::LoadBMap (char * bmpResource, HBITMAP &hBitMap, BITMAP &bitMap)
{
   Component::LoadBMap (bmpResource, hBitMap, bitMap );          
   Component::LoadBMap ("DEPRESSED", hbmDepressed, bmDepressed);
   
   output->LoadBMap (g_hInst);
   input->LoadBMap (g_hInst);
   
}

Pin * MomentaryDepress::PortSelected(){
  Pin * pin = 0;
  if (input->isSelected)
  	pin = input;
  if (output->isSelected)
  	pin = output;
  return pin;  
}

void MomentaryDepress::PaintStart ( HDC & _hdcWindow, HDC & _hdcMemory, PAINTSTRUCT &_ps)
{
  Component::PaintStart ( _hdcWindow, _hdcMemory, _ps);
  
  input->PaintStart ( _hdcWindow, _hdcMemory, _ps);
  output->PaintStart ( _hdcWindow, _hdcMemory, _ps);
}

// [_x,_y] are absolute values
void MomentaryDepress::MoveTo (int _x, int _y)
{
  x = _x-xOffset;
  y = _y-yOffset; // Get the x location of the LED after adjusting for mouse click location
  input->MoveTo (x + input->xOffset - input->bm.bmWidth/2, 
                 y + input->yOffset - input->bm.bmHeight/2);
  output->MoveTo (x + output->xOffset - output->bm.bmWidth/2,
                  y + output->yOffset - output->bm.bmHeight/2);   
}



