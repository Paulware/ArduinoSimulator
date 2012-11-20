#include <windows.h>
#include "SerialClass.h"
#include "EEPROM.h"
#include <Wire.h>
#include "Led.h"
#include "ArduinoComponent.h"
#include "HighLevelMenu.h"
#include "KeyboardMonitor.h"
#include "MomentaryDepress.h"
#include "SevenSeg.h"
#include "LCDDisplay.h"

#define ID_Exit 101
#define ID_Edit 201
#define ID_Load 301
#define ID_Save 302
#define ID_Run  303

#define MYCLASS "ListBox1"

// static char   text[2048];
static HINSTANCE BCX_hInstance = NULL;
static HINSTANCE g_hInst = NULL;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
SerialClass Serial;
EEPROMClass EEPROM;
TwoWire Wire;

// Declare Utility Windows
HighLevelMenu highLevelMenu;
KeyboardMonitor keyboardMonitor;

// Declare components
ArduinoComponent arduino;
Led led1;
Led led2;
Led led3;
MomentaryDepress switch1;
SevenSeg number1;
LCDDisplay lcdDisplay;

/* Definitions of Arduino utilities/externals */
char pgm_read_byte ( char * ch) {return *ch;};
unsigned long millisValue = 0;
unsigned long millis() { 
  return millisValue++;
}
void delay (int milliseconds){}
int analogRead (int which){return 0;}
float log (float value){return 0.0;}
void digitalWrite (int pin, int value ) 
{
  arduino.digitalWrite ( pin, value );   
}
void pinMode (int pin, int mode)
{
  int p = pin;
}
int bitRead ( unsigned int value, int bit )
{
  int val = 0;
  unsigned int values[] = {
    1,     2,     4,     8,     0x10, 0x20, 0x40, 0x80, 
    0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000 };
    
  if (value & values[bit]) 
    val = 1;
  return val;                
}
void lcdPrint ( char * ch )
{
  lcdDisplay.print ( ch );
}
void lcdClear ()
{
  lcdDisplay.clearTheText = true;
}

#include "serialBasic.ino" // Code under test

// Connect the circuit and draw the components
// TODO: Make a Gui for this?
void VirtualBreadBoard (HINSTANCE hInst)
{
  arduino.gnd->Connect(led1.gnd );
  led1.gnd->WriteValue (0,false);
  arduino.d[6]->Connect (led1.power);
  arduino.d[5]->Connect (led2.power);
  switch1.input->Connect (arduino.power);
  switch1.output->Connect (led3.power);
  led2.gnd->Connect(led3.gnd);
  number1.gnd ->WriteValue (0,false);
  
  arduino.d[7]->Connect  ( number1.segment[0]);
  arduino.d[8]->Connect  ( number1.segment[1]);
  arduino.d[9]->Connect  ( number1.segment[2]);
  arduino.d[10]->Connect ( number1.segment[3]);
  arduino.d[11]->Connect ( number1.segment[4]);
  arduino.d[12]->Connect ( number1.segment[5]);
  arduino.d[13]->Connect ( number1.segment[6]);
  

  // Draw the additional components (Arduino is drawn in WinMain)
  led1.DrawWindow       ("Led 1",    hInst,  500,  10);
  led2.DrawWindow       ("Led 2",    hInst,  675,  10);
  led3.DrawWindow       ("Led 3",    hInst,  850,  10);
  switch1.DrawWindow    ("Sw1",      hInst, 1025,  10);
  number1.DrawWindow    ("Segment1", hInst, 1025, 150);
  lcdDisplay.DrawWindow ("Display1", hInst, 925, 300);
}

// Note: component needs to be added here to be displayed
void PaintComponents(HWND hWnd)
{
  // Draw all components
  arduino.Paint (hWnd);
  led1.Paint (hWnd);
  led2.Paint (hWnd);
  led3.Paint (hWnd);
  switch1.Paint (hWnd);
  number1.Paint (hWnd);
  lcdDisplay.Paint (hWnd);
}

void HandleMouseDown(HWND hWnd)
{
  switch1.HandleMouseDown (hWnd);
}

void HandleMouseUp(HWND hWnd)
{
  switch1.HandleMouseUp (hWnd);
}

// this is the main() function under Windows GUI
int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrev,LPSTR CmdLine,int CmdShow)
{ 
 WNDCLASS Wc;
 MSG      Msg; 
 BCX_hInstance    =  hInst;
 HWND hWnd;
 
 // ******************************************************
 Wc.style         =  CS_HREDRAW | CS_VREDRAW | LBS_WANTKEYBOARDINPUT;
 Wc.lpfnWndProc   =  WndProc;
 Wc.cbClsExtra    =  0;
 Wc.cbWndExtra    =  0;
 Wc.hInstance     =  hInst;
 Wc.hIcon         =  LoadIcon(NULL,IDI_WINLOGO);
 Wc.hCursor       =  LoadCursor(NULL,IDC_ARROW);
 Wc.hbrBackground =  (HBRUSH)(COLOR_BTNFACE+1);
 Wc.lpszMenuName  =  NULL;
 Wc.lpszClassName =  MYCLASS;
 RegisterClass(&Wc);

 // Draw all the windows
 (void) highLevelMenu.DrawWindow ( "High Level Menu", hInst, 20, 10);
 (void) keyboardMonitor.DrawWindow ( "Keyboard Monitor", hInst, 20,120);
 hWnd = arduino.DrawWindow ( "Simulated Arduino", hInst, 455, 245 );

 // Connect the circui
 VirtualBreadBoard(hInst);
 
 // Setup timer for calling loop
 SetTimer(hWnd,1,5,NULL); // Will create a WM_Timer message Every 5 ms
  
 // the event message loop
 while(GetMessage(&Msg,NULL,0,0))
   {
    HWND hActiveWindow = GetActiveWindow();
    if (!IsWindow(hActiveWindow) || !IsDialogMessage(hActiveWindow,&Msg))
      {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
      }
    }
 return Msg.wParam; 
}

// standard windows message handler
LRESULT CALLBACK WndProc (HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  static char lastKey = 0;
  static TCHAR szText[512] = "";
  static unsigned long timerCount = 0;
  int len;
             
  switch (Msg) {
    case WM_CREATE:
      break;  
        
    case WM_PAINT:
      PaintComponents(hWnd);
      break;  
   
    case WM_TIMER:
      if (!timerCount++) 
        setup(); // Call setup once in .ino file
      
      loop(); // Call loop in the .ino file
      break;           
      
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
      HandleMouseDown(hWnd);   
      break;        
      
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
      HandleMouseUp(hWnd);   
      break;        
                
    case WM_COMMAND:
      if (LOWORD (wParam) == 11) // Edit Dialog
        if (HIWORD(wParam) == EN_UPDATE)
        {
          keyboardMonitor.GetWindowTxt (szText);
          len = strlen (szText);
          for (int i=0; i<len; i++)
          {
            if (szText[i] == '\r') 
            {                           
              Serial.println ( &szText[0] );
              Serial.saveLine ( &szText[0]);               
              break;
            }  
          }
        }  
      //  list box item clicked (selected) 
      if (LOWORD(wParam)==1009) {}
                       
      if (LOWORD(wParam)==ID_Exit)
        PostQuitMessage(0);
        
      // do something with other menu items here, 
      // normally one calls up a file load/save dialog box
      if (LOWORD(wParam)==ID_Load) 
      {
        //SetWindowText(Form1,"Load ...");
        MessageBox(hWnd, "TBD Not yet connected", "TBD", MB_OK | MB_ICONEXCLAMATION);  
      }
      if (LOWORD(wParam)==ID_Save)  {
        //SetWindowText(Form1,"Save ...");
        MessageBox(hWnd, "TBD Not yet connected", "TBD", MB_OK | MB_ICONEXCLAMATION);  
      }
      
      if (LOWORD (wParam) == ID_Run) {
        //SetWindowText(Form1,"Run it!");
        MessageBox(hWnd, "TBD Not yet connected", "TBD", MB_OK | MB_ICONEXCLAMATION);  
      }
      return 0;
      break;  
        
      /*     
      case WM_KEYDOWN:
        break;  
      case WM_KEYUP:
        break;       
      */
    }   
    
  
  // tidy up and exit the program via the form's upper left corner x 
  if (Msg==WM_DESTROY) {
     UnregisterClass(MYCLASS,BCX_hInstance);
     PostQuitMessage(0);
  }
  
  return DefWindowProc(hWnd,Msg,wParam,lParam);
}
