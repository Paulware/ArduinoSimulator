#include <windows.h>
#include "SerialClass.h"
#include "EEPROM.h"
#include <Wire.h>
#include "Led.h"
#include "ArduinoComponent.h"
#include "HighLevelMenu.h"
#include "KeyboardMonitor.h"

#define ID_Exit 101
#define ID_Edit 201
#define ID_Load 301
#define ID_Save 302
#define ID_Run  303

#define MYCLASS "ListBox1"

static char   text[2048];
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

/* Definitions of Arduino utilities */
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

#include "serialBasic.ino" // Code under test

// Connect the circuit and draw the components
// TODO: Make a Gui for this?
void VirtualBreadBoard (HINSTANCE hInst)
{
  arduino.gnd->Connect(led1.gnd );
  led1.gnd->Connect(led2.gnd);
  arduino.d[6]->Connect (led1.power);
  arduino.d[5]->Connect (led2.power);

  // Draw the additional components 
  led1.DrawWindow ( "Led 1", hInst, 500, 10);
  led2.DrawWindow ( "Led 2", hInst, 675, 10);
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
 hWnd = arduino.DrawWindow ( "Simulated Arduino", hInst, 505, 245 );

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
      // Draw all components
      arduino.Paint (hWnd);
      led1.Paint (hWnd);
      led2.Paint (hWnd);
      break;  
   
    case WM_TIMER:
      if (!timerCount++) 
        setup(); // Call setup once in .ino file
      
      loop(); // Call loop in the .ino file
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
      case WM_LBUTTONDOWN:
        //SetWindowText(Form2,"Left button down");
        break;        
      case WM_RBUTTONDOWN:
        //SetWindowText(Form2,"Right button down");
        break;        
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
