#include "Pin.h"
Pin::Pin(Component * _parent)
{ 
  value = -1;
  connectedTo = 0;
  parent = _parent;
}


int Pin::GetValue ()
{ 
  return value;
}

void Pin::WriteValue(int val, bool recursive)
{
  value = val;
  if (connectedTo && recursive)
    connectedTo->WriteValue ( val, false);
  parent->Refresh();  
}

void Pin::Connect( Pin * pin )
{
  if (!connectedTo)
  {
    connectedTo = pin;      // I'm connected to you
    pin->Connect (this); // You are connected to me
    if (value != -1)
      pin->WriteValue (value, false );
  }  
}
