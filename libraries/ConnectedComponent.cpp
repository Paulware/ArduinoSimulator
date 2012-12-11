#include "ConnectedComponent.h"
ConnectedComponent::ConnectedComponent(int _x, int _y):Component()
{
  x = _x;
  y = _y;
  for (int i=0; i<CONNECTIONSPERCOMPONENT; i++)
    connections[i] = 0;
  numConnections = 0;  
}

ConnectedComponent::~ConnectedComponent()
{
  Connection * connection;	
  int index = 0;
  while ( connection = connections[index++])
    delete (connection);
}

void ConnectedComponent::PaintStart (HDC & _hdcWindow, HDC &_hdcMemory, PAINTSTRUCT & ps)
{
  Connection * connection;
  int index = 0;
  
  Component::PaintStart (_hdcWindow, _hdcMemory, ps );
  while (connection = connections[index++])
  	connection ->PaintStart (_hdcWindow, _hdcMemory, ps); 
}

void ConnectedComponent::DeleteConnection (Pin * pin)
{
  int index = 0;
  Connection * connection;
  bool shifting = false;
  
  while ((shifting || !index) && numConnections)
  {
  	shifting = false;
  	index = 0;
    while ((connection = connections[index]) && numConnections)
    {
  	  if (!shifting && ((connection->pin1 == pin) || (connection->pin2 == pin)))
  	  {
  	  	numConnections--;
  	    delete (connection);
  	    shifting = true;
      }
      if (shifting)
        connections[index] = connections[index+1];
  	  index++;
    }
  }  
}

// All pins should be updated before this procedure is caled
void ConnectedComponent::Move()
{
  int index = 0;
  Connection * connection;
  while (connection = connections[index++])
    connection->Move ();
}

void ConnectedComponent::Paint (HWND hWnd)
{
  Connection * connection;
  int index = 0;
  
  Component::Paint (hWnd);
  while (connection = connections[index++])
  	connection ->Paint(hWnd); 
}

// Set all the pins that can be determined
void ConnectedComponent::SetPins()
{
  int index = 0;
  char * name1;
  char * name2;
  int val1;
  int val2;
  Connection * connection;
  while (connection = connections[index++])
  {
  	name1 = connection->pin1->name;
  	name2 = connection->pin2->name;
  	val1 = connection->pin1->GetValue();
  	val2 = connection->pin2->GetValue();
    if ((val1 != -1) && (val2 == -1))
      connection->pin2->WriteValue(val1);
    else if ((val1 == 0 ) && (val2 == 1))
	  connection->pin2->WriteValue(0); // Gnd trumps value
	  
    if ((val2 != -1) && (val1 == -1))
      connection->pin1->WriteValue(val2);
    else if ((val2 == 0 ) && (val1 == 1))
	  connection->pin1->WriteValue(0); // Gnd trumps voltage
  }
}

void ConnectedComponent::Connect (Pin * pin1, Pin * pin2, HINSTANCE hInst)
{
  Connection * connection = new Connection (this, pin1, pin2);
  connection->LoadBMap ( hInst);	
  connections[numConnections++] = connection;
    
  pin1->Select (false); 
  pin2->Select (false); 
  
  Refresh(); 
}

void ConnectedComponent::SaveConnections (FILE * fp)
{
  Connection * connection;
  int index = 0;
  
  while (connection = connections[index++])
  	connection ->SaveConnection (fp);
}
