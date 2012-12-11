#include "LCDDisplay.h"

LCDDisplay::LCDDisplay(int _x, int _y): ConnectedComponent(_x,_y)
{ 
  offOn = false;
  x = _x;
  y = _y;
  for (int i=0; i<80; i++)
    screen[i] = ' ';
  clearTheText = false; 
  
  for (int i=0; i<MAX_LCD_PINS; i++)
  {
    pin[i] = new Pin(this);
	pin[i]->WriteValue (0);
	if (i<3)  
	  pin[i]->xOffset = 175 + i*11;
	else if (i<9)
	  pin[i]->xOffset = 174 + i*12;  
	else 
	  pin[i]->xOffset = 172 + i*12;  
	pin[i]->yOffset = 265;
	pin[i]->x = x + pin[i]->xOffset;
	pin[i]->y = y + pin[i]->yOffset;
  }	 
  SaveType ("LCDDisplay");   
}

void LCDDisplay::print ( int value)
{
  char val[] = "000000000000";
  itoa (value, val, 10);
  print (val);
}

void LCDDisplay::LoadBMap (char * bmpResource, HBITMAP &hBitMap, BITMAP &bitMap )
{
  Component::LoadBMap (bmpResource, hBitMap, bitMap );     	
  Component::LoadBMap ("REDLED", hbmRedDot, bmRedDot);
  Component::LoadBMap ("BLACKLED", hbmBlackDot, bmBlackDot);
  
  //Component::LoadBMap ("LCDPIN", hbmPinDot, bmPinDot);
  for (int i=0; i<MAX_LCD_PINS; i++)	
  	pin[i]->LoadBMap (g_hInst);
}

void LCDDisplay::SaveYourself (FILE * fp)
{
  fprintf ( fp, "LCDDisplay,%d,%d",x,y);
}

void LCDDisplay::clear()
{
  TextOut (hdcWindow, x+55, y+ 73,  "                    ", 20 );
  TextOut (hdcWindow, x+55, y+103,  "                    ", 20 );
  TextOut (hdcWindow, x+55, y+133,  "                    ", 20 );
  TextOut (hdcWindow, x+55, y+163,  "                    ", 20 );
}

void LCDDisplay::showScreen(HDC hdcWindow)
{ 
  char line[20];
  int index;
  
  clear();  
  for (int i=0; i<4; i++)
  {
    index = i*20;
    for (int j=0; j<20; j++)
    {
      line[j] = screen[index+j];
    }
    TextOut ( hdcWindow, x+55, y+73 + (30*i), &line[0], 20);
  }  
}

void LCDDisplay::AddMenu ()
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

void LCDDisplay::HandleMenu ( int command )
{
  switch (command)
  {
    case LCDDISPLAY_ABOUT:
      MessageBox(windowHandle, "This item can be purchased from ebay for about $7.00 (Free shipping).  Search ebay for \"20x4 LCD Module\".  It has a 16 pin interface.  A $15.00 pcb is available from Tindie.com which has the pins broken out and can be programmed like an Arduino.  Search www.tindie.com  for\n \"20x4 LCD Module\"", "HD44780", MB_OK | MB_ICONEXCLAMATION);  
    break;
    
    case LCDDISPLAY_HELP:
      MessageBox(windowHandle, "LCDDisplay Help", "HD44780", MB_OK | MB_ICONEXCLAMATION);  
    break;
  }   
}

void LCDDisplay::print (char * line)
{
  int len = strlen (line);
  int index;
  static int col = 0;
  static int row = 0;
  
  
  if (clearTheText)
  {
    for (int i=0; i<80; i++)
    {
      screen[i] = ' ';
      col = 0;
      row = 0;
    }
    clearTheText = false;
  }  
  
  index  = col + (row*20);
  
  index %= 80;  
  for (int i=0; i<len; i++)
  {
    screen[index++] = line[i];
    col++;
    if (col == 20)
    {
      col = 0;
      row ++;
      if ( row == 4)
        row = 0;
    }
    index %= 80;
  }
  Refresh();
}

LCDDisplay::~LCDDisplay()
{	
}

void LCDDisplay::Paint(HWND hWnd)
{
  bool ledOn; 
  HFONT hFont;
   
  if (hWnd == windowHandle)
  {
    Component::Paint (hWnd); // Show LCD image  

    
    hFont = CreateFont (28,0,0,0,FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
    CLIP_DEFAULT_PRECIS, 0, VARIABLE_PITCH, TEXT ( "Courier New"));
    (void) SetTextColor ( hdcWindow, RGB (255,255,255));
    (void) SetBkMode( hdcWindow, TRANSPARENT);
    SelectObject (hdcWindow, hFont);
    showScreen(hdcWindow);
    
  
    // Paint the hotspots
    for (int i=0; i<MAX_LCD_PINS; i++)
      pin[i]->Paint(hdcMemory,hdcWindow);
  }    
}

void LCDDisplay::HandleMouseDown (HWND hWnd, int _x, int _y)
{
  Component::HandleMouseDown (hWnd,_x,_y);
  for (int i=0; i<MAX_LCD_PINS; i++)
    if (pin[i]->isActive)
      pin[i]->Select (!pin[i]->isSelected);
}
// Check if the mouse has moved over one of the ports.
void LCDDisplay::HandleMouseMove (HWND hWnd, int _x, int _y)
{
  for (int i=0; i<MAX_LCD_PINS; i++)
    pin[i]->HandleMouseMove (hWnd, _x, _y);
}

Pin * LCDDisplay::PinActive ()
{
  Pin * p;
  for (int i=0; i<MAX_LCD_PINS; i++)
    if (pin[i]->isActive)
	{
	  p = pin[i];
	  break;
	}	
  return p;
}

// [_x,_y] are absolute values
void LCDDisplay::MoveTo (int _x, int _y)
{	
  x = _x-xOffset;
  y = _y-yOffset; // Get the x location of the LED after adjusting for mouse click location
  for (int i=0; i<MAX_LCD_PINS; i++)
  {
    pin[i]->MoveTo ( x + pin[i]->xOffset - pin[i]->bm.bmWidth/2,
                     y + pin[i]->yOffset - pin[i]->bm.bmHeight/2);
  }
  // Move connections
  ConnectedComponent::Move (); 
}

Pin * LCDDisplay::PortSelected(){
  Pin * p = 0;
  for (int i=0; i<MAX_LCD_PINS; i++)
  if (pin[i]->isSelected)
  {
  	p = pin[i];
  	break;
  }	
	  
  return p;  
}

void LCDDisplay::PaintStart ( HDC & _hdcWindow, HDC & _hdcMemory, PAINTSTRUCT &_ps)
{
  Component::PaintStart ( _hdcWindow, _hdcMemory, _ps);
  
  for (int i=0; i<MAX_LCD_PINS; i++)
    pin[i]->PaintStart ( _hdcWindow, _hdcMemory, _ps);
}

