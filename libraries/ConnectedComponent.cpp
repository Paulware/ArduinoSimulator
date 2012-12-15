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

void ConnectedComponent::CleanUp()
{
  Connection * connection;	
  int index = 0;
  Component::CleanUp();
  while ( connection = connections[index++])
    connection->CleanUp();
}

Pin * ConnectedComponent::FindOtherPin (Pin * pin)
{
  Connection * connection;
  int index = 0;
  Pin * otherPin = 0;
  while (connection = connections[index++])
    if (otherPin = connection->OtherPin (pin))
	  break;  
	  
  return otherPin;	
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

void ConnectedComponent::Paint (HDC _hdc, PAINTSTRUCT _ps, HDC _hdcMemory)
{
  Connection * connection;
  int index = 0;
  
  Component::Paint (_hdc, _ps, _hdcMemory);
  
  while (connection = connections[index++])
  {
  	connection ->Paint(hdc, ps, hdcMemory );
  }
  
}
void ConnectedComponent::Init (HWND _windowHandle, HINSTANCE _g_hInst, char * resource) {
  Component::Init (_windowHandle, _g_hInst, resource);
  // TODO: Call Init for the connection or maybe do this when created
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

void ConnectedComponent::Connect (Pin * pin1, Pin * pin2)
{
  Connection * connection = new Connection (pin1, pin2);
  connection->Init (windowHandle, g_hInst);
   
  pin1->Select (false); 
  pin2->Select (false);   
  
  connections[numConnections++] = connection;
}

void ConnectedComponent::SaveConnections (FILE * fp)
{
  Connection * connection;
  int index = 0;
  
  while (connection = connections[index++])
  	connection ->SaveConnection (fp);
}
