#include "SerialClass.h"
#include "stdlib.h"
//typedef char boolean
//#define PGM_P char *
/* 
typedef  char * PGM_P
PGM_P * PSTR ( char * line) 
{
  return ( &line[0] ); 
}
*/
#define PSTR (char *)
#define PGM_P char *
#define prog_char char
#define OUTPUT 1
#define INPUT 0
extern char pgm_read_byte ( char * );
extern SerialClass Serial;
extern unsigned long millis();
extern void digitalWrite ( int, int);
extern void delay (int);
extern int analogRead (int);
extern float log (float);
extern void pinMode (int, int);
extern int bitRead (unsigned int, int);
