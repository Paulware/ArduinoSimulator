#include "MomentaryDepress.h"
#include "HighLevelMenu.h"
MomentaryDepress::MomentaryDepress(int _x, int _y):ConnectedComponent(_x,_y)
{ 
  input = new Pin();
  output = new Pin();
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
  SaveType ("MomentaryDepress"); 
  depressed = false;           
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
  ConnectedComponent::HandleMouseMove (hWnd,_x,_y);
  input->HandleMouseMove  (hWnd, _x, _y );
  output->HandleMouseMove (hWnd, _x, _y );  
}

Pin * MomentaryDepress::FindPort ( char * port)
{
  Pin * pin;
  if (!strcmp (port,"Switch Left"))
    pin = input;
  if (!strcmp(port,"Switch Right"))
    pin = output;
  return pin;    
}

void MomentaryDepress::SaveYourself (FILE * fp)
{
  fprintf ( fp, "MomentaryDepress,%d,%d",x,y);
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
  Refresh();
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

void MomentaryDepress::SetPins()
{
  ConnectedComponent::SetPins();
  if (depressed)
    Pin::BestValue (input->value,output->value);  
}

void MomentaryDepress::HandleMouseUp (HWND hWnd)
{
  if ((hWnd == windowHandle) && !PinActive())
    Reset();
  depressed = false;  
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

void MomentaryDepress::CleanUp()
{
  ConnectedComponent::CleanUp();
  input->CleanUp();
  output->CleanUp();
}

void MomentaryDepress::Paint(HDC _hdc, PAINTSTRUCT _ps, HDC _hdcMemory)
{
  if (depressed)   
    Component::Init (windowHandle, g_hInst, "DEPRESSEDSWITCH");
  else
    Component::Init (windowHandle, g_hInst, "MOMENTARYDEPRESS");
  ConnectedComponent::Paint (_hdc, _ps, _hdcMemory); // Show image    
    
  // Paint the hotspot
  input->Paint(hdc, ps, hdcMemory);
  output->Paint(hdc, ps, hdcMemory);    
}

void MomentaryDepress::Init (HWND _windowHandle, HINSTANCE _g_hInst, char * resource)
{   
   ConnectedComponent::Init ( _windowHandle, _g_hInst, resource);
   output->Init (windowHandle, g_hInst);
   input->Init (windowHandle, g_hInst);   
}

Pin * MomentaryDepress::PortSelected(){
  Pin * pin = 0;
  if (input->isSelected)
  	pin = input;
  if (output->isSelected)
  	pin = output;
  return pin;  
}

// [_x,_y] are absolute values
void MomentaryDepress::MoveTo (int _x, int _y)
{
	
  x = _x-xOffset;
  y = _y-yOffset; // Get the x location of the LED after adjusting for mouse click location
  input->MoveTo (x + input->xOffset, 
                 y + input->yOffset);
  output->MoveTo (x + output->xOffset,
                  y + output->yOffset);   
  // Move connections
  ConnectedComponent::Move ();                   
}
