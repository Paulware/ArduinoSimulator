#include "KeypadDevice.h"
#include "HighLevelMenu.h"

char KeypadDevice::keypadChars[16];
char KeypadDevice::getKey;
int  KeypadDevice::pinIndex;
byte KeypadDevice::connectedTo[MAX_KEYPAD_PINS];

void KeypadDevice::ConnectPin (byte pin)
{
  if (pinIndex < MAX_KEYPAD_PINS)
    connectedTo[pinIndex++] = pin;
}

bool KeypadDevice::TroubleshootPins ( )
{
  bool ok = true;
  char pinName[] = "d00";
  Pin * otherPin;
  for (int i=0; i<MAX_KEYPAD_PINS; i++)
  {
    otherPin = HighLevelMenu::Instance()->FindOtherPin ( pin[i]);	
    if (otherPin)
    {
      pinName[1] = connectedTo[i] / 10 + '0';
      pinName[2] = connectedTo[i] % 10 + '0';
      if (strcmp (otherPin->name, pinName ))
      {
        MessageBox ( 0,pin[i]->name, "Not connected correctly", 0);
        ok = false;
      }
    }
    else
      MessageBox ( 0, "Could not find pin connected to", pin[i]->name,0);
  }
  return ok;
}

KeypadDevice::KeypadDevice(int _x, int _y): ConnectedComponent(_x,_y)
{
  pinIndex = 0; 
  SHORT xs[] = {27,75,122,168};
  SHORT ys[] = {15,61,108,155};
  SHORT tempX, tempY;
  x = _x;
  y = _y;
  clearTheText = false; 
  char pinName[] = "pin0";
  
  for (int i=0; i<MAX_KEYPADS; i++)
  {
  	pads[i].X = xs[i%4];
  	pads[i].Y = ys[i/4];
  	// debug only
  	tempX = pads[i].X;
  	tempY = pads[i].Y;
  }
  
  for (int i=0; i<MAX_KEYPAD_PINS; i++)
  {
    pin[i] = new Pin(this);
	pin[i]->WriteValue (0);
    pin[i]->xOffset = 64 + i*13;
	pin[i]->yOffset = 196;
	pin[i]->x = x + pin[i]->xOffset;
	pin[i]->y = y + pin[i]->yOffset;
	pinName[3] = i + '0';
	pin[i]->SetName ( pinName);
  }	 
  SaveType ("Keypad");     
}

void KeypadDevice::Init (HWND _windowHandle, HINSTANCE _g_hInst, char * resource)
{   
   ConnectedComponent::Init ( _windowHandle, _g_hInst, resource);
   for (int i=0; i<MAX_KEYPAD_PINS; i++)	
   	 pin[i]->Init (windowHandle, g_hInst);
}

void KeypadDevice::SaveYourself (FILE * fp)
{
  fprintf ( fp, "Keypad,%d,%d",x,y);
}

void KeypadDevice::AddMenu ()
{
  HMENU  MainMenu;
  HMENU  FileMenu;
     
  //arduinoMain.setHwnd ( hwndOwner);
  MainMenu=CreateMenu();
  FileMenu=CreateMenu();
  InsertMenu(MainMenu,ID_Edit,MF_POPUP,(UINT_PTR)FileMenu,"Help");
  AppendMenu(FileMenu,MF_STRING,LCDDISPLAY_ABOUT,"&About");
  AppendMenu(FileMenu,MF_SEPARATOR,0,"");
  AppendMenu(FileMenu,MF_STRING,LCDDISPLAY_HELP,"&Component Help");
  //  activate menu 
  (void) SetMenu(windowHandle,MainMenu);
}

Pin * KeypadDevice::FindPort ( char * port)
{
  char pinName[] = "pin0";
  Pin * p = 0;
  
  for (int i=0; i<MAX_KEYPAD_PINS; i++)
  {
  	pinName[3] = '0' + i;
  	if (!strcmp (port,pinName))
  	{
  	  p = pin[i]; 
  	  break;
  	}
  }
  return p;    
}


void KeypadDevice::HandleMenu ( int command )
{
  switch (command)
  {
    default:
    break;
  }   
}

KeypadDevice::~KeypadDevice()
{
  for (int i=0; i<MAX_KEYPAD_PINS; i++)
    delete (pin[i]);	
}

void KeypadDevice::Paint(HDC _hdc, PAINTSTRUCT _ps, HDC _hdcMemory)
{ 
  ConnectedComponent::Paint (_hdc, _ps, _hdcMemory); // Show image  
     
  // Paint the pins
  for (int i=0; i<MAX_KEYPAD_PINS; i++)
    pin[i]->Paint(hdc, ps, hdcMemory);
}

void KeypadDevice::HandleMouseUp (HWND hWnd)
{
  getKey = 0;
}

void KeypadDevice::HandleMouseDown (HWND hWnd, int _x, int _y)
{
  ConnectedComponent::HandleMouseDown (hWnd,_x,_y);
  int pad = -1;
  char msg[20] = "1 key pressed";
  char ch;
  for (int i=0; i<MAX_KEYPAD_PINS; i++)
    if (pin[i]->isActive)
      pin[i]->Select (!pin[i]->isSelected);
      
  for (int i=0; i<MAX_KEYPADS; i++)
  {
  	if ((_x >= x+pads[i].X) && (_x <= x+pads[i].X + 33) && // within width
  	    (_y >= y+pads[i].Y) && (_y <= y+pads[i].Y + 30))   // within height
  	{
  	  pad = i;
  	  getKey = keypadChars[i];
  	  break;
  	}
  }    
}
// Check if the mouse has moved over one of the ports.
void KeypadDevice::HandleMouseMove (HWND hWnd, int _x, int _y)
{
  for (int i=0; i<MAX_KEYPAD_PINS; i++)
    pin[i]->HandleMouseMove (hWnd, _x, _y);
    
}

Pin * KeypadDevice::PinActive ()
{
  Pin * p;
  for (int i=0; i<MAX_KEYPAD_PINS; i++)
    if (pin[i]->isActive)
	{
	  p = pin[i];
	  break;
	}	
  return p;
}

void KeypadDevice::Select ( bool select)
{
  for (int i=0; i<MAX_KEYPAD_PINS; i++)
    pin[i]->Select(false);
}

// [_x,_y] are absolute values
void KeypadDevice::MoveTo (int _x, int _y)
{	
  x = _x-xOffset;
  y = _y-yOffset; // Get the x location of the LED after adjusting for mouse click location
  for (int i=0; i<MAX_KEYPAD_PINS; i++)
  {
    pin[i]->MoveTo ( x + pin[i]->xOffset, y + pin[i]->yOffset);
  }
  // Move connections
  ConnectedComponent::Move (); 
}

Pin * KeypadDevice::PortSelected(){
  Pin * p = 0;
  for (int i=0; i<MAX_KEYPAD_PINS; i++)
  if (pin[i]->isSelected)
  {
  	p = pin[i];
  	break;
  }	
	  
  return p;  
}

bool KeypadDevice::IsSet()
{
  bool set = true;
  for (int i=0; i<MAX_KEYPAD_PINS; i++)
  {
  	if (!pin[i]->IsSet())
  	{
  	  set = false;
  	  break;
  	}
  }
  return set;
}

void KeypadDevice::Reset()
{
  for (int i=0; i<MAX_KEYPAD_PINS; i++)
    pin[i]->Reset();
}
