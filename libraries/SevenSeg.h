#ifndef SEVENSEG_H
#define SEVENSEG_H
#include "Component.h"
#include "Pin.h"
class SevenSeg:Component
{
  public:          
    // Constructor
    SevenSeg(); 
    // Destructor
    ~SevenSeg();
    HINSTANCE g_hInst; 
    HWND DrawWindow (char * title, HINSTANCE hInst, int x , int y);   
    void Paint(HWND hWnd);                      
    Pin * segment[7];
    Pin * gnd; // Common Ground
    
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


