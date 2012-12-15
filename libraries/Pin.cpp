#include "Pin.h"
#include "HighLevelMenu.h"
Pin::Pin():Component()
{ 
  value.value = -1;
  value.resistance = 0;
  constValue.value = -1;	 
  constValue.resistance = 0;
  
  x = 0;
  y = 0; 
  xOffset = 0;
  yOffset = 0;  
  isActive = false;
  isSelected = false;
  name = 0; 
}

Pin::~Pin()
{
  // Delete all connections that use this pin
  HighLevelMenu::Instance()->DeleteConnection (this);
  delete (name);
}

void Pin::SetName(char * _name)
{
  name = (char *)calloc (1,strlen(_name)+1);
  strcpy ( name,_name);
}

void Pin::Init (HWND _windowHandle, HINSTANCE _g_hInst)
{
  Component::Init (_windowHandle, _g_hInst, "");
} 

void Pin::Paint (HDC _hdc, PAINTSTRUCT _ps, HDC _hdcMemory)
{ 
  if (isActive)   
    Component::Init (windowHandle, g_hInst, "REDDOT");
  else
    Component::Init (windowHandle, g_hInst, "BLACKDOT");
    
  Component::Paint (_hdc, _ps, _hdcMemory);   
}

void Pin::MoveTo (int _x, int _y)
{
  x = _x;
  y = _y;
  Refresh();
}

// Change color of pin to indicate active
void Pin::Select(bool active)
{
  isSelected = active;    
}

// [_x,_y] is absolute not relative to parent
void Pin::HandleMouseMove (HWND hWnd, int _x, int _y)
{
  if ((_x >= x) && (_x <= x+bm.bmWidth) && (_y >= y) && (_y <= y+bm.bmHeight))
    isActive = true;
  else if (isActive)
    isActive = false;    
}

void Pin::Reset()
{
  if (constValue.value == -1)
    value.value = -1;
  else
    value.value = constValue.value;  
    
  if (constValue.resistance != 0)
    value.resistance = constValue.resistance;
  else
    value.resistance = 0;      
}

bool Pin::IsSet()
{
  bool set = true;
  if (GetValue() == -1)
    set = false;
  return set;  
}

int Pin::GetResistance()
{
  return value.resistance;
}

void Pin::BestValue (PinValueType &v1, PinValueType &v2)
{
  if (v1.value != -1) // v1 is set 
    if (v2.value != -1) // v2 is set 
      if (v2.resistance < v1.resistance) // Path of least resistance 
        v1 = v2;
      else if (v1.resistance < v2.resistance) // for debug only
        v2 = v1;
      else 
	    v2 = v1;
	else // v2 is not set 
	  v2 = v1;	  
  else // v1 is not set
    v1 = v2;
}

// Check constValue, actual value or connected value
int Pin::GetValue ()
{ 
  
  int val = -1;
  if (constValue.value != -1)
    val = constValue.value;
  else if (value.value != -1)
    val = value.value;
    
  if (val == 1)
    val = 1;  
  return val;
}

void Pin::WriteValue(int val, int resistance)
{
  value.value = val;
  value.resistance = resistance;
}

void Pin::WriteValue(int val)
{
  value.value = val;
}

bool Pin::IsSelected ()
{
  return isSelected;
}

