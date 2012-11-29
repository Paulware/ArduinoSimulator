#include "HighLevelMenu.h"
#include "Menus.h"
#include "Led.h"
#include "MomentaryDepress.h"
#include "SevenSeg.h"
#include "LCDDisplay.h"
#include "ArduinoComponent.h"
#include "Connection.h"

HighLevelMenu::HighLevelMenu():Component()
{ 
  numComponents = 0;
  numConnections = 0;  
  selectedItem = -1;
  for (int i=0; i<MAX_COMPONENTS; i++)
  {
  	components[i] = 0;
  	connections[i] = 0;
  }
  viewConnections = new ViewConnections (100,100);
}

HighLevelMenu::~HighLevelMenu()
{
  delete (viewConnections);
}

void HighLevelMenu::HandleMouseMove (HWND hWnd, int x, int y)
{  
  Component * component;
  Connection * connection;
  int index = 0;
  
  while (component = components[index++])
  {
  	component->isActive =  (
	    (x >= component->x) && (x <= component->x + component->width) &&
        (y >= component->y) && (y <= component->y + component->height));
    if (component->isActive)
    { 
      // HandleMouseMove shows offset within the window  
      component->HandleMouseMove (hWnd, x,y);
      break;
    }  
    
  } 
     
  if (selectedItem > -1)
  {
    components[selectedItem]->MoveTo (x,y);
    components[selectedItem]->Refresh();
  }  
}

// Set the component xOffset and yOffset
void HighLevelMenu::HandleMouseDown (HWND hWnd, int x, int y)
{
  bool isActive = false;
  Component * component;
  int index = 0;
  
  while (component = components[index])
  {
    if ((x >= component->x) && (x <= component->x + component->width) &&
        (y >= component->y) && (y <= component->y + component->height))
    {   
      component->xOffset = x - component->x;
      component->yOffset = y - component->y;
      if (!component->PortSelected() || !component->PinActive())
      {
        component->HandleMouseDown(hWnd, x, y);               
        selectedItem = index;
      }
      else // Toggle the pin activity
      {
        if ((component->PortSelected() != component->PinActive()))
        {
          component->Select(false);
          component->HandleMouseDown(hWnd, x, y);               
          selectedItem = index;        	
        }  
        else 
          component->Select(false);
      }      
      break;
    }  
    index++;
  } 
  Refresh();   
}

void HighLevelMenu::ComputeSystem()
{
  Component * component;
  int index = 0;
  int numSet;
  int lastSet = 0;
  // Reset the system
  while (component = components[index++])
  {
  	component->Reset();
  }
  
  // Set the system
  while (true)
  {
  	numSet = 0;
  	index = 0;
  	while (component = components[index++])
  	{
      if (component->IsSet())
  	    numSet++;
  	}
  	// Check if no changes this loop
  	if (numSet == lastSet) 
  	  break;
  	lastSet = numSet;  
  }  
  
}

void HighLevelMenu::HandleMouseUp (HWND hWnd)
{
  int numSelected = 0;
  Connection * c;
  struct IdPort
  {
  	COORD port;
  	int componentIndex;
  };
  Pin * pin;
  Pin * pin1 = 0;
  Pin * pin2;
  
  Component * component;
  int index = 0;
  
  ComputeSystem();
  
  while (component = components[index])
  {
  	if (component->isActive)
      component->HandleMouseUp (hWnd);
      
    if (pin = (Pin *)component->PortSelected())
    {
      if (!pin1) // This is the first port selected
        pin1 = pin;
      else
        pin2 = pin;
      numSelected++;
    }
    index++;
  }
  
  if (numSelected == 2)
  {
  	pin1->ConnectDots ( pin2); // not recursive
  	pin1->Connect (pin2);
  }
  selectedItem = -1; // Stop tracking the mouse      
  Refresh();   
}

void HighLevelMenu::AddMenu ()
{     
  HMENU  MainMenu, hSubMenu;
  
  MainMenu = CreateMenu();
  SetMenu (windowHandle, MainMenu); 
  
  hSubMenu = CreatePopupMenu();
  AppendMenu (hSubMenu, MF_STRING, ADDLED,             "Led" );
  AppendMenu (hSubMenu, MF_STRING, ADDMOMENTARYSWITCH, "Depress Switch");
  AppendMenu (hSubMenu, MF_STRING, ADDSEVENSEGMENT,    "7Segment Display");
  AppendMenu (hSubMenu, MF_STRING, ADDLCDDISPLAY,      "LCD Display");
  AppendMenu (hSubMenu, MF_STRING, ADDARDUINO,         "Arduino");
  InsertMenu (MainMenu, 0,         MF_POPUP|MF_BYPOSITION, (UINT_PTR)hSubMenu, "Add");
  
  hSubMenu = CreatePopupMenu();
  AppendMenu (hSubMenu, MF_STRING, VIEWARDUINOCONNECTIONS, "Arduino Connections");
  InsertMenu ( MainMenu, 0, MF_POPUP|MF_BYPOSITION, (UINT_PTR)hSubMenu, "View");
    
  hSubMenu = CreatePopupMenu();
  AppendMenu (hSubMenu, MF_STRING, 1003, "New");
  AppendMenu (hSubMenu, MF_STRING, 1003, "Open");
  AppendMenu (hSubMenu, MF_STRING, 1004, "Save");
  AppendMenu (hSubMenu, MF_STRING, 1005, "Exit");
  InsertMenu ( MainMenu, 0, MF_POPUP|MF_BYPOSITION, (UINT_PTR)hSubMenu, "File");
  
  //  activate menu 
  (void) SetMenu(windowHandle,MainMenu);
}

// Set the hdcWindow, hdcMemory and ps for all the components 
void HighLevelMenu::PaintStart ()
{
  hdcWindow = 0;
  hdcMemory = 0;
  Component::PaintStart (hdcWindow, hdcMemory, ps);
  Component * component;
  Connection * connection;
  int index = 0;
  
  while (component = components[index++])	
  	component->PaintStart (hdcWindow, hdcMemory, ps);
  
  index = 0;
}

void HighLevelMenu::Paint(HWND hWnd)
{
  int index = 0;
  Component * component;
  Connection * connection;
  	
  if (hWnd == windowHandle)
  {
    if (numComponents || numConnections)
    {
      PaintStart ();
      while (component = components[index++])
        component->Paint (hWnd);
      PaintEnd ();
    }  
  }   
}

HWND HighLevelMenu::DrawWindow (char * title, HINSTANCE hInst, char * bmpResource, 
                                int x , int y, int width, int height)
{                            
  HWND hWnd = Component::DrawWindow(title, hInst, "", x, y, 1200,1100);
  AddMenu();
  return hWnd;
}

void HighLevelMenu::HandleMenu (int command)
{
  switch (command)
  {
    case ADDLED: 
      components[numComponents] = new Led(0,0);       
      components[numComponents++]->Show ( g_hInst, windowHandle, "LEDBMP" );
    break;
    case ADDMOMENTARYSWITCH:
      components[numComponents] = new MomentaryDepress(100,100);  
      components[numComponents++]->Show (g_hInst, windowHandle, "MOMENTARYDEPRESS");              
    break;    
    case ADDSEVENSEGMENT:
      components[numComponents] = new SevenSeg(100,100); 
      components[numComponents++]->Show (g_hInst, windowHandle, "SEVENSEGMENT" );
    break;  
    case ADDLCDDISPLAY:
      components[numComponents] = new LCDDisplay(00,00);  
      components[numComponents++]->Show (g_hInst, windowHandle, "LCDDISPLAY" );
    break;  
    case ADDARDUINO:
      components[numComponents] = new ArduinoComponent (00,00);
      components[numComponents]->Show (g_hInst,windowHandle, "ARDUINOBMP");
      viewConnections->AddDevice ( "Arduino",components[numComponents++] );
    break;
    
    case VIEWARDUINOCONNECTIONS:
      viewConnections->DrawWindow (g_hInst);
    break;          
  }   
}


