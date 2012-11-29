#include "Connection.h"
#include "Menus.h"
#include "Line.h"

Connection::Connection(COORD port1, COORD port2)
{ 
  line = new Line (port1.X, port1.Y, port2.X, port2.Y);
}

void Connection::LoadBMap (HINSTANCE hInst )
{
  if (line)
    line->LoadBMap (hInst);
}

void Connection::PaintStart ( HDC & _hdcWindow, HDC & _hdcMemory, PAINTSTRUCT &_ps)
{	
  line->PaintStart ( _hdcWindow, _hdcMemory, _ps);
}

void Connection::MoveTo(int x1, int y1, int x2, int y2)
{
  line->MoveTo (x1, y1, x2, y2);
}

Connection::~Connection()
{
  delete (line);
}
