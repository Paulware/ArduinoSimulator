#include "Resistor.h"
#include "HighLevelMenu.h"
Resistor::Resistor(int _x, int _y, int _resistance):ConnectedComponent(_x,_y)
{ 
  input = new Pin(this);
  output = new Pin(this);
  input->xOffset = 0;
  input->yOffset = 5;
  output->xOffset = 67;
  output->yOffset = 5; 
  x = _x;
  y = _y;
  input->x = x + input->xOffset;
  input->y = y + input->yOffset;    
  output->x = x + output->xOffset;
  output->y = y + output->yOffset; 
  input->SetName ("Left");
  output->SetName ("Right");  
  SaveType ("Resistor");          
}

Resistor::~Resistor()
{
	
  HighLevelMenu::Instance()->DeleteConnection (input);
  HighLevelMenu::Instance()->DeleteConnection (output);
	
  delete (input);
  delete (output);
}

Pin * Resistor::PinActive ()
{
  Pin * pin = 0;
  if (input->isActive)
    pin = input;
  else if (output->isActive)
    pin = output;
  return pin;
}

// Check if the mouse has moved over one of the ports.
void Resistor::HandleMouseMove (HWND hWnd, int _x, int _y)
{
  ConnectedComponent::HandleMouseMove (hWnd,_x,_y);
  input->HandleMouseMove  (hWnd, _x, _y );
  output->HandleMouseMove (hWnd, _x, _y );  
  input->value.resistance = resistance;
  output->value.resistance = resistance;
}

Pin * Resistor::FindPort ( char * port)
{
  Pin * pin = 0;
  if (!strcmp (port,"Left"))
    pin = input;
  if (!strcmp(port,"Right"))
    pin = output;
  return pin;    
}

void Resistor::SaveYourself (FILE * fp)
{
  fprintf ( fp, "Resistor,%d,%d",x,y);
}

void Resistor::HandleMouseDown (HWND hWnd, int _x, int _y)
{
  if (input->isActive) // we are over the ground spot     
    input->Select(!input->isSelected);
  else if (output->isActive)
    output->Select(!output->isSelected);  
  else
  {
    if (output->GetValue() != -1)
      input->WriteValue ( output->GetValue(), resistance);
    else if (input->GetValue () != -1)
	  output->WriteValue ( input->GetValue(), resistance);  
  }  
}

bool Resistor::IsSet()
{
  bool set = input->IsSet () && output->IsSet();
  return set;
}

void Resistor::Reset ()
{
  input->Reset();
  output->Reset();
}

void Resistor::SetPins()
{
  ConnectedComponent::SetPins();
  if (input->GetValue() == -1)
    input->WriteValue(output->GetValue(), output->GetResistance());
  if (output->GetValue() == -1)
    output->WriteValue(input->GetValue(), output->GetResistance());    
}

void Resistor::HandleMouseUp (HWND hWnd)
{
  /*
  if ((hWnd == windowHandle) && !PinActive())
  {
    // Reset();
  }
  */
}


void Resistor::AddMenu ()
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

void Resistor::Paint(HWND hWnd)
{
  bool connected; 
  int buttonX = 29;
  int buttonY = 10;
    
  if (hWnd == windowHandle)
  {
    ConnectedComponent::Paint (hWnd); // Show Arduino image    
    
    // Paint the hotspot
    input->Paint(hdcMemory,hdcWindow);
    output->Paint(hdcMemory,hdcWindow);
    
  }  
}

void Resistor::LoadBMap (char * bmpResource, HBITMAP &hBitMap, BITMAP &bitMap)
{
   // ConnectedComponent::LoadBMap (bmpResource, hBitMap, bitMap );          
   ConnectedComponent::LoadBMap ("TENKOHMS", hBitMap, bitMap );          
   
   output->LoadBMap (g_hInst);
   input->LoadBMap (g_hInst);   
}

Pin * Resistor::PortSelected(){
  Pin * pin = 0;
  if (input->isSelected)
  	pin = input;
  if (output->isSelected)
  	pin = output;
  return pin;  
}

void Resistor::PaintStart ( HDC & _hdcWindow, HDC & _hdcMemory, PAINTSTRUCT &_ps)
{
  ConnectedComponent::PaintStart ( _hdcWindow, _hdcMemory, _ps);
  
  input->PaintStart ( _hdcWindow, _hdcMemory, _ps);
  output->PaintStart ( _hdcWindow, _hdcMemory, _ps);
}

// [_x,_y] are absolute values
void Resistor::MoveTo (int _x, int _y)
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

