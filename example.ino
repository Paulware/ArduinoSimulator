#include <ArduinoBASIC.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <SevenSegment.h>

ArduinoBASIC arduinoBASIC = ArduinoBASIC ();
DebugUtilities debugUtils;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
SevenSegment sevenSegment (2,3,4,5,6,13,14);

#define NUMBER_OF_STEPS 25
PSTRStrings eProgram = PSTRStrings(NUMBER_OF_STEPS);
bool paused;

void callback (int value)
{
  Serial.print ( "From callback value:" );
  Serial.println ( value );
}

unsigned long runTimeout;
void setup()
{  
  bool d10;
  Serial.begin (115200);
  debugUtils.printPSTR ( PSTR ( "Arduino BASIC...enter ? to display keywords\nTo use the Arduino IDE\n select 'Carriage return' rather than 'no line ending' in the Serial Monitor\n" ));
  arduinoBASIC.init();
  arduinoBASIC.eepromProgram.callback = callback;
    
  eProgram.addString ( PSTR ( "lcdDisplayInit"));  
  eProgram.addString ( PSTR ( ":loop"));
  eProgram.addString ( PSTR ( "ifA")); 
  eProgram.addString ( PSTR ( "lcdDisplayClear"));
  eProgram.addString ( PSTR ( "lcdDisplayPrintA is set"));
  eProgram.addString ( PSTR ( "setA=0" )); 
  eProgram.addString ( PSTR ( "else"));
  eProgram.addString ( PSTR ( "lcdDisplayClear"));
  eProgram.addString ( PSTR ( "lcdDisplayPrintA is not set"));
  eProgram.addString ( PSTR ( "setA=1"));
  eProgram.addString ( PSTR ( "endif"));
  eProgram.addString ( PSTR ( "delay20"));
  eProgram.addString ( PSTR ( "jumploop"));

  runTimeout = millis() + 10000;
  Serial.println ( "Program will run in 10 seconds unless a key is pressed" );    
  paused = true;  
  
  lcd.begin(20, 4); // Specify how many columns and rows in the LCD unit
  lcd.clear();
  lcd.print("HD44780 LCD Ready   ");
  d10 = digitalRead(10);
}

void loop()
{
  static unsigned long runTimeout;
  static bool last10 = false;
  static int buttonCount = 0;
  static unsigned long blinkyTime =0;
  static int blinky = 0;
  bool d10;
  static int segValue = 0;
  
  if (millis() > blinkyTime)
  {
  	blinkyTime = millis() + 1000;
  	blinky = 1 - blinky;
  	digitalWrite (15,blinky);
  }
  
  if (runTimeout)
  {
    if (millis() > runTimeout)
    {
      arduinoBASIC.eepromProgram.run();
      runTimeout = 0;
      paused = false;
    }
  }

  d10 = digitalRead(10);
  if (!d10 && last10)
  {
  	buttonCount++;
  	Serial.print ( "Detected button press ");
  	Serial.println ( buttonCount );
  	lcd.print ( buttonCount % 10);
    sevenSegment.print(buttonCount %10);  	
  }
  last10 = d10;
  
      
  if (!paused)  
    arduinoBASIC.eepromProgram.continueTest();
    
  if (Serial.available())
  {
    runTimeout = 0;
    paused = false;
    arduinoBASIC.handleChar(eProgram, Serial.read()); 
  }  
}

