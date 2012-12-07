#ifndef SEVENSEG_H
#define SEVENSEG_H
#include "ConnectedComponent.h"
class SevenSeg:public ConnectedComponent
{
  public:          
    // Constructor
    SevenSeg(int,int); 
    // Destructor
    ~SevenSeg(); 
    void LoadBMap (char * bmpResource, HBITMAP &hBitMap, BITMAP &bitMap );
    HWND DrawWindow (char * title, HINSTANCE hInst, char * bmpResource, 
                     int x , int y, int width, int height);
    void PaintStart ( HDC & _hdcWindow, HDC & _hdcMemory, PAINTSTRUCT &_ps);
    void Paint(HWND hWnd);                      
    void AddMenu();
    void HandleMenu ( int command );
    void HandleMouseMove (HWND hWnd, int _x, int _y);  
    Pin * PortSelected();
    void HandleMouseDown (HWND hWnd, int _x, int _y);  
    void MoveTo (int, int);  
    Pin * PinActive ();      
    Pin * segment[7];
    Pin * gnd; // Common Ground
    void SaveYourself (FILE * fp);
    void Select ( bool select);
    Pin * FindPort ( char *);     
    void Reset();
    
  private:
    // bool offOn;
    HBITMAP       hbmUpDown;
    BITMAP        bmUpDown;

    HBITMAP       hbmLeftRight;
    BITMAP        bmLeftRight;

    struct SegmentInfo
    {
      int x;
      int y;
      bool horizontalVertical;
    };
    SegmentInfo segmentValues[7];    

};

#endif


