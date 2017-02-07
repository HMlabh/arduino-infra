/*
 Name:		arduino-infra.ino
 Created:	07.02.2017 14:40:44
 Author:	gutek
*/


//------------------Defines-------------------
#define serialDebug //Debug output over Serial connection to PC



//------------------Pinset--------------------

namespace pin
{
	int SCL = 3;
	int SDA = 2;
	int EN0 = 11;
	int EN1 = 10;
	int EN2 = 9;
	int EN3 = 8;
	int EN4 = 7;
	int EN5 = 6;
	int EN6 = 5;
	int EN7 = 4;
	int EN8 = 19;
	int EN9 = 18;
	int EN10 = 13;
	int EN11 = 12;
	int EN12 = 23;
	int EN13 = 22;
	int EN14 = 21;
	int EN15 = 20;
}
namespace addr
{
	int AD0 = 0x48;
	int AD1 = 0x49;
	int AD2 = 0x4A;
	int AD3 = 0x4B;
}

//-------------------Init-----------------------
int enables[16] =
{ pin::EN0 , pin::EN1 , pin::EN2 , pin::EN3 , pin::EN4 , 
  pin::EN5 , pin::EN6 , pin::EN7 , pin::EN8 , pin::EN9 , 
  pin::EN10 , pin::EN11 , pin::EN12 , pin::EN13 , pin::EN14 , pin::EN15};

int measurecount = 0;

//-------------------Setup-----------------------

void setup() 
{
#ifdef serialDebug				//establish Serial Debug-Connection
	Serial.begin(115200);
	while (!Serial) 
	{
		; // wait for serial port to connect. Needed for native USB port only
	}
#endif // serialDebug



	//Pinmode Enables
	for (int i = 0; i <= 15; i++)
	{
		pinMode(enables[], OUTPUT); //Sensor 0 Enable 0utput
	}
	

}

//-------------------Functions------------------




//-------------------Loop--------------------

void loop() 
{
	#ifdef serialDebug

	#endif // serialDebug

	

	measurecount++;
	//delay(10);

}
