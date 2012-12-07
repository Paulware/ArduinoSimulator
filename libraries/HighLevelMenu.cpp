#include "HighLevelMenu.h"
#include "Menus.h"
#include "Led.h"
#include "MomentaryDepress.h"
#include "SevenSeg.h"
#include "LCDDisplay.h"
#include "ArduinoComponent.h"
#include "Connection.h"
#include <stdio.h>
#include <Commdlg.h>


static HighLevelMenu * highLevelMenu;
HighLevelMenu::HighLevelMenu(ViewConnections * _viewConnections):Component()
{ 
  numComponents = 0;  
  selectedItem = -1;
  for (int i=0; i<MAX_COMPONENTS; i++)
  	components[i] = 0;
  
  viewConnections = _viewConnections;
  strcpy(projectFilename,"");
  highLevelMenu = this;
}

HighLevelMenu::~HighLevelMenu()
{
}

// Singleton pointer to class
HighLevelMenu * HighLevelMenu::Instance()
{
  return highLevelMenu;
}

ConnectedComponent * HighLevelMenu::FindComponent ( char * typeName)
{
  int index = 0;
  int found = -1;
  ConnectedComponent * component = 0;
  
  while (component=components[index])
  {
    if (!strcmp ( component->componentType, typeName))
    {
      found = index;
      break;
    }
    index++;
  }
    
  if (found == -1)
    component = 0;
    
  return component;
}

int HighLevelMenu::DigitalRead(int pin)
{
  int value = 0;
  ArduinoComponent * arduino;
  
  if (arduino = (ArduinoComponent *)FindComponent ( "Arduino"))
    value = arduino->d[pin]->GetValue();
    
  if (value == 1)
    value = 1;  
    
  return value; 	
}

void HighLevelMenu::DigitalWrite(int pin, int value)
{
  ArduinoComponent * arduino;
  
  if (arduino = (ArduinoComponent *)FindComponent ( "Arduino"))
  {
  	Reset();
    arduino->d[pin]->constValue = value;
    ComputeSystem();
    Refresh();
  }
}

void HighLevelMenu::lcdPrint(char * line)
{
  LCDDisplay * lcdDisplay;
  
  if (lcdDisplay = (LCDDisplay *)FindComponent ( "LCDDisplay"))
    lcdDisplay->print(line);
}

void HighLevelMenu::lcdClear()
{
  LCDDisplay * lcdDisplay;
  
  if (lcdDisplay = (LCDDisplay *)FindComponent ( "LCDDisplay"))
    lcdDisplay->clear();
}

void HighLevelMenu::lcdPrint ( int value)
{
  LCDDisplay * lcdDisplay;
  
  if (lcdDisplay = (LCDDisplay *)FindComponent ( "LCDDisplay"))
    lcdDisplay->print(value);
}

void HighLevelMenu::HandleMouseMove (HWND hWnd, int x, int y)
{  
  ConnectedComponent * component;
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
    index = 0;
    while (component=components[index++])
      component->Move();
  }  
}

void HighLevelMenu::Reset ()
{
  ConnectedComponent * component;
  int index = 0;
  
  // Set all values to -1
  while (component = components[index++])
  	component->Reset();  
}

// Set the component xOffset and yOffset
void HighLevelMenu::HandleMouseDown (HWND hWnd, int x, int y)
{
  bool isActive = false;
  ConnectedComponent * component;
  int index = 0;

  Reset();

  while (component = components[index])
  {
    if ((x >= component->x) && (x <= component->x + component->width) &&
        (y >= component->y) && (y <= component->y + component->height))
    {   
      component->xOffset = x - component->x;
      component->yOffset = y - component->y;
      // Make the port like a radio button so that only 1 port is selectable
      // at a time.
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
        else // Deselect the port
          component->Select(false);
      }      
      break;
    }  
    index++;
  } 
  
  ComputeSystem();
  Refresh();   
}

void HighLevelMenu::ComputeSystem()
{
  Connection * connection;
  int numSet;
  int lastSet = 0;
  int index = 0;
  ConnectedComponent * component;
  
  // Set the system
  while (true)
  {
  	numSet = 0;
  	index = 0;
  	while (component = components[index++])
  	{
  	  component->SetPins ();
  	}
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
  
  // As many pins as possible are set.
  
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
  
  ConnectedComponent * component1;
  ConnectedComponent * component2;
  ConnectedComponent * component;
  int index = 0;
  
  // Set all values to -1
  Reset();
  
  index = 0;    
  while (component = components[index])
  {
  	if (component->isActive)
      component->HandleMouseUp (hWnd);
      
    if (pin = (Pin *)component->PortSelected())
    {
      if (!pin1) // This is the first port selected
      {
        pin1 = pin;
        component1 = component;
      }
      else
      {
        pin2 = pin;
        component2 = component;
      }
      numSelected++;
      if (numSelected ==2)
      {
        component1->Connect (pin1, pin2, g_hInst);
      }
    }
    index++;
  }
  
  ComputeSystem();
  selectedItem = -1; // Stop tracking the mouse      
  Refresh();   
}

void HighLevelMenu::AddMenu ()
{     
  HMENU  MainMenu, hSubMenu, hSubSubMenu;
  
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
  AppendMenu (hSubMenu, MF_STRING, NEWPROJECT, "New Project");
  AppendMenu (hSubMenu, MF_STRING, READPROJECT, "Open Project");

  hSubSubMenu = CreatePopupMenu();
  AppendMenu (hSubSubMenu, MF_STRING, SAVE, "Save");
  AppendMenu (hSubSubMenu, MF_STRING, SAVEAS, "Save As");
  InsertMenu (hSubMenu, 0,         MF_POPUP|MF_BYPOSITION, (UINT_PTR)hSubSubMenu, "Save Project");
  
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
  ConnectedComponent * component;
  int index = 0;
  
  Component::PaintStart (hdcWindow, hdcMemory, ps);
  
  while (component = components[index++])	
  	component->PaintStart (hdcWindow, hdcMemory, ps);
}

void GetToken ( FILE * fp, char * line)
{
  char ch = 0;	
  int len = 0;
  line[0]=0;
  while (!feof(fp))
  {
  	ch = fgetc (fp);
  	if ((ch == ',') || (ch == '\n') || (ch == -1))
  	  break;
  	else
  	{
  	  line[len++]=ch;
  	  line[len]=0;
    }
  }
}

class OpenFileDialog {
	public: 
	  OpenFileDialog ()
	  {
	  };
	  
	  void Open (const char * filter)
	  {
        // open a file name
	    ZeroMemory( &ofn , sizeof( ofn));
	    ofn.lStructSize = sizeof ( ofn );
    	ofn.hwndOwner = NULL  ;   
	    ofn.lpstrFile = szFile ;
   	    ofn.lpstrFile[0] = '\0';
	    ofn.nMaxFile = sizeof( szFile );
	    // ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	    //ofn.lpstrFilter = "Text\0*.TXT\0";
	    ofn.lpstrFilter=filter;
	    ofn.nFilterIndex =1;
	    ofn.lpstrFileTitle = NULL ;
	    ofn.nMaxFileTitle = 0 ;
	    ofn.lpstrInitialDir=NULL ;
	    ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;
	    GetOpenFileName( &ofn );
	    // Now simpley display the file name 
	    // MessageBox ( NULL , ofn.lpstrFile , "File Name" , MB_OK);
	  };
	  
	  void Save (const char * filter)
	  {
        // open a file name
	    ZeroMemory( &ofn , sizeof( ofn));
	    ofn.lStructSize = sizeof ( ofn );
    	ofn.hwndOwner = NULL  ;   
	    ofn.lpstrFile = szFile ;
   	    ofn.lpstrFile[0] = '\0';
	    ofn.nMaxFile = sizeof( szFile );
	    ofn.lpstrFilter=filter;
	    ofn.nFilterIndex =1;
	    ofn.lpstrFileTitle = NULL ;
	    ofn.nMaxFileTitle = 0 ;
	    ofn.lpstrInitialDir=NULL ;
	    ofn.Flags = OFN_PATHMUSTEXIST;
	    GetSaveFileName( &ofn );
	  }
	  char * Filename ()
	  {
	  	return &szFile[0];
	  }
	private:
  	  OPENFILENAME ofn ;
  	  char szFile[100] ;
};

/*
class String{
   private:
      enum { SZ = 80 };
      char str[SZ];
   public:
      String(){ strcpy(str, ""); }
      String( char s[] ){ strcpy(str, s); }
      void display() const{ MessageBox (0,str,"show",0); }
      //void getstr(){ cin.get(str, SZ); }
      bool operator == (String ss) const{
         return ( strcmp(str, ss.str)==0 ) ? true : false;
      }
      String& operator=(const String& String) {
      if(this == &String)
        return *this;

      delete[] pString;
      pString = new char[ strlen(String.pString) + 1];

      // Copy right operand string to left operand
      std::strcpy(this->pString, String.pString);

      return *this;
    }

      
   };

*/
const int MAXCHARS = 25;
typedef char LineType[MAXCHARS];
struct LineInfo
{
  LineType component1;
  int x1;
  int y1;
  LineType port1;
  
  LineType component2;
  int x2;
  int y2;
  LineType port2;
  bool newData;
  bool readingComponent;
};

LineInfo DefaultLine ()
{
  LineInfo line;
  strcpy (line.component1,"");
  line.x1 = 0;
  line.y1 = 0;
  strcpy (line.port1,"");
  strcpy (line.component2,"");
  line.x2 = 0;
  line.y2 = 0;
  strcpy (line.port2,"");
  line.newData = false;
  line.readingComponent = false;
  return line;
}

void ReadLine (FILE * fp, LineInfo & line)
{
  static bool readingComponents;
  char token[MAXCHARS];
  GetToken (fp,&token[0]);
  line = DefaultLine ();
  line.readingComponent = readingComponents;
  if (strlen(token))
  {
    line.newData = true;  
    if (!strcmp(token,"Project Components"))
      readingComponents = true;
    else if (!strcmp(token,"Project Connections"))	
      readingComponents = false;
    else if (readingComponents)
    { 
      strcpy (line.component1, token);
      GetToken (fp,&token[0]); // x
      line.x1 = atoi (&token[0]);
      GetToken (fp,&token[0]); // y
	  line.y1 = atoi (&token[0]);	
    }
    else // Reading Connections
    {
      strcpy ( line.component1, token);
      GetToken (fp,&token[0]); // x
      line.x1 = atoi (&token[0]);
      GetToken (fp,&token[0]); // y
      line.y1 = atoi (&token[0]);
      GetToken (fp,&token[0]); // portName
      strcpy (line.port1, token);
      GetToken (fp,&token[0]); // Component
      strcpy (line.component2, token); 
      GetToken (fp,&token[0]); // x
      line.x2 = atoi(&token[0]);
      GetToken (fp,&token[0]); // y
      line.y2 = atoi(&token[0]);
      GetToken (fp,&token[0]); // portName
      strcpy (line.port2, token); 
    }
  }
}

ConnectedComponent * HighLevelMenu::FindComponent (int x, int y)
{
  int index = 0;
  ConnectedComponent * component;
  while (component = components[index++])
  {
  	if ((component->x == x) && (component->y == y))
  	{
  	  break;
  	}
  }
  return component;
}

void HighLevelMenu::NewProject()
{
  ConnectedComponent * component;
  int index = 0;
  // Delete all the components
  while (component = components[index])
  {
    delete (component);
    components[index++] = 0;
  }
  numComponents = 0;
  Refresh();	
}

void HighLevelMenu::AddComponent (char * typeName, int x, int y)
{
  //SevenSeg * sevenSegment;	
  //ArduinoComponent * arduino;
  if (!strcmp ( "Arduino",typeName))
  {
 	components[numComponents] = new ArduinoComponent ( x, y);
    components[numComponents++]->Show (g_hInst, windowHandle, "ARDUINOBMP" );
  }
  else if (!strcmp ("Led", typeName))
  {
    components[numComponents] = new Led ( x, y);
    components[numComponents++]->Show (g_hInst, windowHandle, "LEDBMP" );
  }
  else if (!strcmp ("MomentaryDepress", typeName))
  {
    components[numComponents] = new MomentaryDepress ( x, y);
    components[numComponents++]->Show (g_hInst, windowHandle, "MOMENTARYDEPRESS");	
  }
  else if (!strcmp ("LCDDisplay", typeName))
  {
    components[numComponents] = new LCDDisplay ( x, y);
    components[numComponents++]->Show (g_hInst, windowHandle, "LCDDISPLAY");	
  }
  else if (!strcmp ("Seven Segment", typeName))
  {
    components[numComponents] = new SevenSeg ( x, y);
    components[numComponents++]->Show (g_hInst, windowHandle, "SEVENSEGMENT");	
    // byte sevenPins[]={2,3,4,5,6,13,14};      
    // MessageBox ( 0,"Now connect pins", "",0);
    // sevenSegment = (SevenSeg *)components[numComponents++];
    
    /*
    arduino = (ArduinoComponent *)FindComponent ("Arduino");
    arduino->Connect (arduino->d[2], sevenSegment->segment[0], g_hInst);
    arduino->Connect (arduino->d[3], sevenSegment->segment[1], g_hInst);
    arduino->Connect (arduino->d[4], sevenSegment->segment[2], g_hInst);
    arduino->Connect (arduino->d[5], sevenSegment->segment[3], g_hInst);
    arduino->Connect (arduino->d[6], sevenSegment->segment[4], g_hInst);
    arduino->Connect (arduino->d[13], sevenSegment->segment[5], g_hInst);
    arduino->Connect (arduino->d[14], sevenSegment->segment[6], g_hInst);
    */ 
  }
  else
    MessageBox (0,"Could not find a component",typeName,0);
}

void HighLevelMenu::ReadProject(char * filename)
{ 	
 FILE * fp;	
 Connection * connection;
 ConnectedComponent * component;
 ConnectedComponent * component1;
 ConnectedComponent * component2;
 LineInfo line;
 int len;
 int index = 0;
 Pin * port1;
 Pin * port2;
 
 
 if (fp = fopen (filename,"r"))
 {
   NewProject();	
   strcpy ( projectFilename,filename);	
   while (!feof(fp))
   {	  	
 	 ReadLine ( fp, line );	
 	 if (line.newData)
 	 {
 	   if(strlen ( line.component1))
 	     if (line.readingComponent)
 	  	   AddComponent (line.component1, line.x1, line.y1);	 	  	    
 	  	 else // reading a connection
 	  	 {
 	  	   component1 = FindComponent (line.x1,line.y1);
 	  	   component2 = FindComponent (line.x2,line.y2);
 	  	   if (!component1)
 	  	  	 MessageBox (0,"Can't find component1","Oops",0);
 	  	   else if (!component2)
 	  	  	 MessageBox (0,"Can't find component2", "Oops", 0);
 	  	   port1 = (Pin *)component1->FindPort (line.port1);
 	  	   if (!port1)
 	  	  	 MessageBox (0,"Can't find port1","Oops", 0);
 	  	   
 	  	  	  
 	  	   port2 = (Pin *)component2->FindPort (line.port2);
 	  	   if (!port2)
 	  	     MessageBox (0,"Can't find port2","Oops",0);
 	  	   else
 	  	   {
             component1->Connect (port1, port2, g_hInst);
  	       }
 	  	 }
 	 
	 }
 	 else
 	   break;
  }
  fclose (fp);
 }
}

void HighLevelMenu::NotSavedYet ()
{
  ConnectedComponent * component;	
  int index = 0;
  while (component=components[index++])
    component->NotSavedYet();
}
    
void HighLevelMenu::SaveProject(char * filename)
{
  OpenFileDialog ofd;
  int index = 0;
  FILE *fp;
  ConnectedComponent * component;

  NotSavedYet();
  
  if (strlen(filename))
    fp = fopen (filename, "w");
  else
  {
    ofd.Save("Text\0*.TXT\0");
    fp = fopen(ofd.Filename(), "w");
  }
  
  if (fp)
  {
  	if (components[0])
  	{
  	  fprintf (fp, "Project Components\n");
      while (component = components[index++])
      {
        component->SaveYourself (fp);  
	    fprintf (fp, "\n");
      }
       
      fprintf (fp, "Project Connections\n");
      index = 0;
  	  while (component=components[index])
  	  {
	    component->SaveConnections(fp);
		index++; 
      }
    }
    fclose (fp);
  }
  MessageBox ( 0,"Project Saved", "Save", 0);
}

void HighLevelMenu::Paint(HWND hWnd)
{
  int index = 0;
  ConnectedComponent * component;
  Connection * connection;
  	
  //if (hWnd == windowHandle)
  //{
    PaintStart ();
    while (component = components[index])
    {
      component->Paint (hWnd);
      index++;
    } 
    PaintEnd ();
  //}   
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
  OpenFileDialog ofd;
		
  switch (command)
  {
    case ADDLED: 
      AddComponent ("Led",0,0);
    break;
    
    case ADDMOMENTARYSWITCH:
      AddComponent ("MomentaryDepress",0,0);
    break;   
	 
    case ADDSEVENSEGMENT:
      AddComponent ("Seven Segment",0,0);
    break;  
    
    case ADDLCDDISPLAY:
      AddComponent ("LCDDisplay",0,0);
    break;
	  
    case ADDARDUINO:
      AddComponent ("Arduino",0,0);
    break;
    
	case SAVEAS:
	  ofd.Save("Text\0*.TXT");		
	  if (strlen (ofd.Filename()))
	    SaveProject (ofd.Filename());
	break;   	
    
    case VIEWARDUINOCONNECTIONS:
      viewConnections->DrawWindow (g_hInst);
    break; 
	
	case SAVE: 
	  SaveProject(projectFilename);
	break;
	
	case READPROJECT:
	  ofd.Open("Text\0*.TXT");		
	  ReadProject(ofd.Filename());
	break;
	
	case NEWPROJECT:
	  NewProject();
	break;
	  
  }   
}

