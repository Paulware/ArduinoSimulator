#include "ViewConnections.h"
#include "Pin.h"
#include "ArduinoComponent.h"
#include "HighLevelMenu.h"

extern HWND viewConnectionHandle; // definition outside class declaration

ViewConnections::ViewConnections(int _x, int _y):Component()
{ 
  x = _x;
  y = _y;
}

ViewConnections::~ViewConnections()
{ 	
}

// Set the hdcWindow, hdcMemory and ps for all the components 
void ViewConnections::PaintStart ()
{
  hdcWindow = 0;
  hdcMemory = 0;
  Component::PaintStart (hdcWindow, hdcMemory, ps);
}

void ViewConnections::Paint(HWND hWnd)
{
  int x;
  int y=0;	
  Connection * connection;
  ArduinoComponent * arduino = (ArduinoComponent *)HighLevelMenu::Instance()->FindComponent("Arduino");
  char * name;
  Pin * pin;
  char value[10];
  /*
  char * names[numConnections] = {    "d0",  "d1",  "d2",  "d3", "d4", "d5", "d6", "d7", "d8", "d9",
                                    "d10", "d11", "d12", "d13", "a0", "a1", "a2", "a3", "a4", "a5", 
   				   			      "power", "gnd"};
  char * info[numConnections] = { "NC", "NC", "NC", "NC", "NC", "NC", "NC", "NC", "NC", "NC",
                                  "NC", "NC", "NC", "NC", "NC", "NC", "NC", "NC", "NC", "NC", 
   				   			      "NC", "NC"};			   			      
  */   				   			      
  int index = 0;   				   			      
  PaintStart ();

  while (connection = arduino->connections[index])
  {
  	name = connection->pin1->name;
    TextOut (hdcWindow,  10, y,name,strlen(name));
    
    name = connection->pin2->name;
    TextOut (hdcWindow, 100, y,name,strlen(name));
    
    itoa (connection->pin1->GetValue(), value, 10);
    TextOut (hdcWindow, 190, y, value, strlen (value));
    
    itoa (connection->pin2->GetValue(), value, 10);
    TextOut (hdcWindow, 280, y, value, strlen (value));
    y += 20;
    index++;
  }
  
  PaintEnd ();
}

HWND ViewConnections::DrawWindow (HINSTANCE hInst)
{                            
  HWND hWnd = Component::DrawWindow("Arduino Connections", hInst, "", x, y, 500,500);
  viewConnectionHandle = hWnd;
  Paint (windowHandle);
  // AddMenu();
  return hWnd;
}
