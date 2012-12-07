#include "Connection.h"
Connection::Connection(Component * _component1, Pin * _pin1, Pin * _pin2)
{ 
  pin1 = _pin1;
  pin2 = _pin2;
  component1 = _component1;
  x1 = pin1->x;
  y1 = pin1->y;
  x2 = pin2->x;
  y2 = pin2->y;
  line = new Line (x1,y1,x2,y2);
}

void Connection::LoadBMap (HINSTANCE hInst )
{
  if (line)
    line->LoadBMap (hInst);
}

void Connection::Paint ( HWND hWnd)
{
  Component::Paint (hWnd);
  line->Paint (hWnd);
}

void Connection::PaintStart ( HDC & _hdcWindow, HDC & _hdcMemory, PAINTSTRUCT &_ps)
{	
  Component::PaintStart ( _hdcWindow, _hdcMemory, _ps);
  line->PaintStart ( _hdcWindow, _hdcMemory, _ps);
}

void Connection::Move()
{  	
  x1 = pin1->x;
  y1 = pin1->y;
  x2 = pin2->x;
  y2 = pin2->y;
  line->MoveTo (x1, y1, x2, y2);
}

Connection::~Connection()
{
  if (line)
    delete (line);
}

void Connection::SaveConnection(FILE * fp)
{
  pin1->parent->SaveYourself (fp);
  fprintf ( fp, ",%s,", pin1->name);
  pin2->parent->SaveYourself (fp);
  fprintf ( fp, ",%s\n", pin2->name);
}
