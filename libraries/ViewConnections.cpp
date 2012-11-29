#include "ViewConnections.h"
#include "Pin.h"

extern HWND viewConnectionHandle; // definition outside class declaration


ViewConnections::ViewConnections(int _x, int _y):Component()
{ 
  x = _x;
  y = _y;
  for (int i=0; i<numConnections;i++)
    connections[i] = 0;
  numDevices = 0; 
}

void ViewConnections::AddDevice ( char * name, Component * component)
{
 devices[numDevices].component = component;
 devices[numDevices].name = (char *) calloc ( 1, strlen(name)+ 1);
 strcpy ( devices[numDevices++].name, name);
}

ViewConnections::~ViewConnections()
{
  for (int i=0;i<numConnections;i++)
    delete (connections[i]);
    
  for (int i=0;i<numDevices;i++)  
  	delete (devices[i].name);
  	
}

void ViewConnections::SetConnection (int i, char * value)
{
  	
  if (i<numConnections)
  {
  	delete (connections[i]);
  	connections[i] = (char *)calloc (1,strlen(value)+1);
  }
  strcpy (connections[i],value);
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
  int numConnects;
  Component * component;
  char * name;
  Pin * pin;
  char * names[numConnections] = {    "d0",  "d1",  "d2",  "d3", "d4", "d5", "d6", "d7", "d8", "d9",
                                    "d10", "d11", "d12", "d13", "a0", "a1", "a2", "a3", "a4", "a5", 
   				   			      "power", "gnd"};
  char * info[numConnections] = { "NC", "NC", "NC", "NC", "NC", "NC", "NC", "NC", "NC", "NC",
                                  "NC", "NC", "NC", "NC", "NC", "NC", "NC", "NC", "NC", "NC", 
   				   			      "NC", "NC"};
  PaintStart ();
  for (int i=0; i<numDevices; i++)
  {
  	if (!strcmp ( devices[i].name,"Arduino"))
  	{
  	  component = devices[i].component;
  	  numConnects = component->NumConnections();
  	  for (int j=0; j<numConnections; j++)
      {
      	pin = (Pin *)component->GetConnection(j);
      	if (pin->connection)
      	{
          //delete(info[j]);      
          name = pin->ConnectedName();
		  if (name)
		  {	
            info[j] = (char *) calloc ( 1,strlen(name)+1);
            strcpy ( info[j],name);
          }
          else // store empty string in info[j]
          	info[j] = (char *) calloc ( 1,1);
        }
      }
  	  break;
  	}
  }
  
  for (int i=0; i<numConnections; i++)
  {
    TextOut (hdcWindow,  10, y,names[i],strlen(names[i]));
    TextOut (hdcWindow, 100, y,info[i],strlen(info[i]));
    y += 20;
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
