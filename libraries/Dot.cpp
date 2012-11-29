#include "Dot.h"
Dot::Dot(int _x, int _y):Component()
{ 
  x = _x;
  y = _y;
}

Dot::~Dot()
{
}

void Dot::HandleMouseDown (HWND hWnd, int _x, int _y)
{
  Component::HandleMouseDown (hWnd,_x,_y);
}

void Dot::HandleMouseMove (HWND hWnd, int _x, int _y)
{
}

// [_x,_y] are absolute values
void Dot::MoveTo (int _x, int _y)
{
  x = _x;
  y = _y;
}                             

void Dot::PaintStart ( HDC & _hdcWindow, HDC & _hdcMemory, PAINTSTRUCT &_ps)
{
  Component::PaintStart ( _hdcWindow, _hdcMemory, _ps);     
}

void Dot::LoadBMap (HINSTANCE hInst)
{
  g_hInst = hInst;
  hbm = LoadBitmap (hInst, "DOT");   
  GetObject(hbm, sizeof(bm), &bm);
}

void Dot::Paint(HWND hWnd)
{   
  Component::Paint (hWnd);   
}
