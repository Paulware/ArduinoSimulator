#include <Keypad.h>
#include <KeypadDevice.h>

Keypad::Keypad(char *userKeymap, byte *row, byte *col, byte numRows, byte numCols) {
    rowPins = row;
    columnPins = col;
    sizeKpd.rows = numRows;
    sizeKpd.columns = numCols;

    begin(userKeymap);

    setDebounceTime(10);
    setHoldTime(500);
	keypadEventListener = 0;

    initializePins();

	startTime = 0;
}

void Keypad::begin(char *userKeymap) {
    keymap = userKeymap;
    for (int i=0; i<16; i++)
      KeypadDevice::keypadChars[i] = keymap[i];
}

// Returns a single key only. Retained for backwards compatibility.
char Keypad::getKey() {
	char ch = KeypadDevice::getKey;
	KeypadDevice::getKey = 0;
	return ch;
}

bool Keypad::getKeys() {
	char ch = KeypadDevice::getKey;
	return ch;
}

bool Keypad::isPressed(char keyChar) {
  bool match = false;
  char ch = KeypadDevice::getKey;
  if (ch == keyChar)
    match = true;
  return match;	// Not pressed.
}

// New in 2.0
char Keypad::waitForKey() {
	while( !KeypadDevice::getKey );	// Block everything while waiting for a keypress.
	return KeypadDevice::getKey;
}

KeyState Keypad::getState() {
	return key[0].kstate;
}

// The end user can test for any changes in state before deciding
// if any variables, etc. needs to be updated in their code.
// Useful for keypads/keyboards returning a single key only.
bool Keypad::keyStateChanged() {
	return key[0].stateChanged;
}

// The number of keys on the key list, key[LIST_MAX], equals the number
// of bytes in the key list divided by the number of bytes in a key.
byte Keypad::numKeys() {
	return sizeof(key)/sizeof(Key);
}

// Minimum debounceTime is 10 mS. Any lower *will* slow down the loop().
void Keypad::setDebounceTime(uint debounce) {
	debounce<1 ? debounceTime=1 : debounceTime=debounce;
}

void Keypad::setHoldTime(uint hold) {
    holdTime = hold;
}

void Keypad::addEventListener(void (*listener)(char)){
	keypadEventListener = listener;
}

void Keypad::transitionTo(byte n, KeyState nextState) {
	key[n].kstate = nextState;
	key[n].stateChanged = true;

	if (keypadEventListener!=NULL)
		keypadEventListener(key[0].kchar);
}

void Keypad::initializePins() {
	// Set columns low and rows high with impedence.
	// This will cause a 0 to be generated when the button is depressed
    for (byte C=0; C<sizeKpd.columns; C++) {
        pin_mode(columnPins[C],OUTPUT);
        pin_write(columnPins[C],LOW);
        
    }    
	for (byte r=0; r<sizeKpd.rows; r++) {
		pin_mode(rowPins[r],INPUT);
		pin_write(rowPins[r],HIGH);	// Enable the internal 20K pullup resistors.
	}    
}

// Search for a key in the list of active keys.
// Returns -1 if not found or the index into the list of active keys.
int Keypad::findInList (char keyChar) {
	for (byte i=0; i<LIST_MAX; i++) {
		if (key[i].kchar == keyChar) {
			return i;
		}
	}
	return -1;	// No matching key in list.
}



