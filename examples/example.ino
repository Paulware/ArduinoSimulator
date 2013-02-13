#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <SevenSegment.h>
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
  
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
SevenSegment sevenSegment (2,3,4,5,6,13,14);
#define BLINKYPIN 15

#define NUMBER_OF_STEPS 25
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
  runTimeout = millis() + 10000;
  Serial.println ( "Load an example project.txt from the High Level Menu to show functionality" );    
  paused = true;  
  
  lcd.begin(20, 4); // Specify how many columns and rows in the LCD unit
  lcd.clear();
  lcd.print("HD44780 LCD Ready   ");
  pinMode (10,INPUT); // Switch
  d10 = digitalRead(10);
  pinMode (BLINKYPIN,OUTPUT); // LED blink
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
    
  char key = keypad.getKey();
  
  if (key){
    Serial.println(key);
  } 
  
  switch (key)
  {
  	case '*':
  	  lcd.clear();
      lcd.print ("A star was pressed");
  	break;
  	
  	case '#':
  	  lcd.clear();
      lcd.print ("A pound was pressed");
  	break;
  	
  	default:
	break;
  }
  
  if (millis() > blinkyTime)
  {
  	blinkyTime = millis() + 1000;
  	blinky = 1 - blinky;
  	digitalWrite (BLINKYPIN,blinky);
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
      
  if (Serial.available())
  {
    runTimeout = 0;
    paused = false;
  }  
  
}

