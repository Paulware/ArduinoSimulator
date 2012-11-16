#include "Pin.h"
Pin::Pin(Component * _parent)
{ 
  value = -1;
  parent = _parent;
  numConnections = 0;
}


int Pin::GetValue ()
{ 
  return value;
}

void Pin::WriteValue(int val, bool recursive)
{
  
  if (value != val) // This is a new value
  {
    value = val;
    for (int i=0; i<numConnections; i++)
      connectedTo[i]->WriteValue ( val, false);
  }    
  parent->Refresh();  
}

void Pin::Connect( Pin * pin )
{
  bool found = false;
  // Check if already connected
  for (int i=0; i<numConnections; i++)
    if (connectedTo[i] == pin)
      found = true;
  
  // Only connect once    
  if (!found)
  {    
    // I am connected to you
    connectedTo[numConnections++] = pin;   
    // You are connected to me
    pin->Connect (this);
    if (value != -1) // I already have a value
      pin->WriteValue (value, false );
  }    
}
