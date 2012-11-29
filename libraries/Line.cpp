#include "Line.h"
#include "Menus.h"

Line::Line(int _x1, int _y1, int _x2, int _y2):Component()
{ 
  float xOffset = 0.0;
  float yOffset = 0.0;
  int x;
  int y;
  x1 = _x1;
  y1 = _y1;
  x2 = _x2;
  y2 = _y2;
  float xDiff = (float)(x2 - x1) / (float)DOTSPERLINE;
  float yDiff = (float)(y2 - y1) / (float)DOTSPERLINE;
  
  for (int i=0; i<DOTSPERLINE; i++)
  {
  	x = x1+xOffset;
  	y = y1+yOffset;
    dot[i] = new Dot (x,y);
    xOffset += xDiff;
    yOffset += yDiff;
  }
}

Line::~Line()
{
}

void Line::HandleMouseDown (HWND hWnd, int _x, int _y)
{
  Component::HandleMouseDown (hWnd,_x,_y);
}

void Line::MoveTo (int _x1, int _y1, int _x2, int _y2)
{
  float xOffset = 0.0;
  float yOffset = 0.0;
  int x;
  int y;
  
  x1 = _x1;
  y1 = _y1;
  x2 = _x2;
  y2 = _y2;
  float xDiff = (float)(x2 - x1) / (float)(DOTSPERLINE-1);
  float yDiff = (float)(y2 - y1) / (float)(DOTSPERLINE-1);
  
  for (int i=0; i<DOTSPERLINE-1; i++)
  {
  	dot[i]->MoveTo (x1+xOffset,y1+yOffset);
  	dot[i]->CenterYourself ();
	xOffset += xDiff;
    yOffset += yDiff;
  }  
  dot[DOTSPERLINE-1]->MoveTo ( x2,y2);
}

void Line::PaintStart ( HDC & _hdcWindow, HDC & _hdcMemory, PAINTSTRUCT &_ps)
{	
  Component::PaintStart ( _hdcWindow, _hdcMemory, _ps);
  for (int i=0; i<DOTSPERLINE; i++)
    dot[i]->PaintStart ( hdcWindow,hdcMemory,ps);      
}

void Line::Paint(HWND hWnd)
{
  bool ledOn; 
   
  // Paint the background
  Component::Paint (hWnd); 
	
  for (int i=0; i<DOTSPERLINE; i++)	
    dot[i]->Paint (hWnd);
}

void Line::LoadBMap (HINSTANCE hInst)
{	
  for (int i=0; i<DOTSPERLINE; i++)
  {
    dot[i]->LoadBMap (hInst);
    dot[i]->CenterYourself();
  }
}

