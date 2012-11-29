#include "Pin.h"
Pin::Pin(Component * _parent)
{ 
  value = -1;
  parent = _parent;
  numConnections = 0; 
  x = 0;
  y = 0; 
  xOffset = 0;
  yOffset = 0;  
  isActive = false;
  isSelected = false;
  connection = 0;
  for (int i=0; i<MAX_PIN_CONNECTIONS; i++)
     connectedTo[i] =0;  
  name = 0; 
  constValue = -1;	       
}

Pin::~Pin()
{
  if (connection)
    delete (connection);
  delete (name);
}

char * Pin::ConnectedName ()
{
  char * name = 0;
  if (connectedTo[0])
    name = connectedTo[0]->name;
  return name;
}

void Pin::SetName(char * _name)
{
  name = (char *)calloc (1,strlen(_name)+1);
  strcpy ( name,_name);
}

void Pin::PaintStart (HDC & _hdcWindow, HDC &_hdcMemory, PAINTSTRUCT & ps)
{
  if (connection)
    connection->PaintStart (_hdcWindow, _hdcMemory, ps);
}

void Pin::Paint (HDC hdcMemory, HDC hdcWindow)
{
  SelectObject(hdcMemory, hbm); 
  BitBlt(hdcWindow, x, y, bm.bmWidth, bm.bmHeight, hdcMemory, 0, 0, SRCAND);
  BitBlt(hdcWindow, x, y, bm.bmWidth, bm.bmHeight, hdcMemory, 0, 0, SRCPAINT);
  if (connection)
    connection->Paint (parent->windowHandle);
}

void Pin::MoveTo (int _x, int _y)
{
  Pin * conn = connectedTo[0];

  x = _x;
  y = _y;
  if (numConnections) // TODO: necessary?  check conn when numConnections > 0
  {
    if (connection && numConnections) // TODO handle more than one connection
      connection->MoveTo (x, y, conn->x, conn->y);
    else if (conn)
      if (conn->connection)
        conn->connection->MoveTo (x, y, conn->x, conn->y);
  }
}

// Change color of pin to indicate active
void Pin::Select(bool active)
{
  if (active)   
    hbm = LoadBitmap (myInst, "REDDOT");   
  else
    hbm = LoadBitmap (myInst, "BLACKDOT"); 
  isSelected = active;    
  GetObject(hbm, sizeof(bm), &bm);    
}

// [_x,_y] is absolute not relative to parent
void Pin::HandleMouseMove (HWND hWnd, int _x, int _y)
{
  if ((_x >= x) && (_x <= x+bm.bmWidth) && (_y >= y) && (_y <= y+bm.bmHeight))
    isActive = true;
  else if (isActive)
    isActive = false;    
}

void Pin::LoadBMap ( HINSTANCE h_Inst)
{
  myInst = h_Inst;
  hbm = LoadBitmap (h_Inst, "BLACKDOT");   
  GetObject(hbm, sizeof(bm), &bm);
  // Center the object on the x/y pixel
}

void Pin::LoadBMap ( HINSTANCE h_Inst, char * bmpResource)
{
  myInst = h_Inst;
  hbm = LoadBitmap (h_Inst, bmpResource);   
  GetObject(hbm, sizeof(bm), &bm);
  // Center the object on the x/y pixel
}

void Pin::Reset()
{
  value = -1;
}

bool Pin::IsSet()
{
  bool set = true;
  if (GetValue() == -1)
    set = false;
  return set;  
}

// Check constValue, actual value or connected value
int Pin::GetValue ()
{ 
  Pin * pin;
  int val = -1;
  if (constValue != -1)
    val = constValue;
  else if (value != -1)
    val = value;
  else
  {
    pin = connectedTo[0];
    if (pin)
      if (pin->constValue != -1)
        val = pin->constValue;
      else
        val = pin->value;
  }
  return val;
}

void Pin::WriteValue(int val)
{
  value = val;
}

bool Pin::IsSelected ()
{
  return isSelected;
}

void Pin::ConnectDots ( Pin * pin)
{
  int xOffset = 0; // bm.bmWidth/2;
  int yOffset = 0; // bm.bmHeight/2;
	
  if (connection)
    delete (connection); // Memory cleanup
  // Only create 1 line from first pin to second
  connection = new Connection ({x+xOffset,y+yOffset},{pin->x+xOffset,pin->y+yOffset});
  connection->LoadBMap (myInst);    
 
  // Deselect both pins
  Select (false); 
  pin->Select (false);
}

void Pin::Connect( Pin * pin )
{
  bool found = false;
  
  if (!connection)
    connection = pin->connection;
    
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
  }  
}
