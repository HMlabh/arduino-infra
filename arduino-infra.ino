/*
 Name:		arduino-infra.ino
 Created:	07.02.2017 14:40:44
 Author:	gutek
*/

#include <Wire.h>
#include <Adafruit_ADS1015.h>
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
long measuretime = 0;

int16_t ranges[16] = { 0 };

//-----ADC-----
Adafruit_ADS1115 ad0(addr::AD0);	// construct an ads1115 at address AD0
Adafruit_ADS1115 ad1(addr::AD1);	// construct an ads1115 at address AD1
Adafruit_ADS1115 ad2(addr::AD2);	// construct an ads1115 at address AD2
Adafruit_ADS1115 ad3(addr::AD3);	// construct an ads1115 at address AD3

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
		pinMode(enables[i], OUTPUT); //Sensor i Enable 0utput
	}
	
	// HACK SEN ON!!!
	for (int i = 0; i <= 15; i++)
	{
		digitalWrite(enables[i], HIGH); //Sensor i HIGH
	}

	//Begin serial communication to ADS1115
	ad0.begin();
	ad1.begin();
	ad2.begin();
	ad3.begin();

	

}

//-------------------Functions------------------

int16_t readsensor(int sensornr)
{
	int16_t range = 0;

	if (sensornr >= 0 && sensornr <= 15)
	{
		switch (sensornr)
		{

		case 0:range = ad0.readADC_SingleEnded(0);break;
		case 1:range = ad0.readADC_SingleEnded(1);break;
		case 2:range = ad0.readADC_SingleEnded(2);break;
		case 3:range = ad0.readADC_SingleEnded(3);break;

		case 4:range = ad1.readADC_SingleEnded(0);break;
		case 5:range = ad1.readADC_SingleEnded(1); break;
		case 6:range = ad1.readADC_SingleEnded(2); break;
		case 7:range = ad1.readADC_SingleEnded(3); break;

		case 8:range = ad2.readADC_SingleEnded(0); break;
		case 9:range = ad2.readADC_SingleEnded(1); break;
		case 10:range = ad2.readADC_SingleEnded(2); break;
		case 11:range = ad2.readADC_SingleEnded(3); break;

		case 12:range = ad3.readADC_SingleEnded(0); break;
		case 13:range = ad3.readADC_SingleEnded(1); break;
		case 14:range = ad3.readADC_SingleEnded(2); break;
		case 15:range = ad3.readADC_SingleEnded(3); break;
		}
		return range;
	}
	else
	{
		return -9999;
	}
}

void readallsensors()
{
	ranges[0] = ad0.readADC_SingleEnded(0);
	ranges[1] = ad0.readADC_SingleEnded(1);
	ranges[2] = ad0.readADC_SingleEnded(2);
	ranges[3] = ad0.readADC_SingleEnded(3);

	ranges[4] = ad1.readADC_SingleEnded(0);
	ranges[5] = ad1.readADC_SingleEnded(1);
	ranges[6] = ad1.readADC_SingleEnded(2);
	ranges[7] = ad1.readADC_SingleEnded(3);

	ranges[8] = ad2.readADC_SingleEnded(0);
	ranges[9] = ad2.readADC_SingleEnded(1);
	ranges[10] = ad2.readADC_SingleEnded(2);
	ranges[11] = ad2.readADC_SingleEnded(3);

	ranges[12] = ad3.readADC_SingleEnded(0);
	ranges[13] = ad3.readADC_SingleEnded(1);
	ranges[14] = ad3.readADC_SingleEnded(2);
	ranges[15] = ad3.readADC_SingleEnded(3);
}

void fastread()
{
	//Start Reading Block 0:
	ad0.readADC_SingleEnded_Start(0);
	ad1.readADC_SingleEnded_Start(0);
	ad2.readADC_SingleEnded_Start(0);
	ad3.readADC_SingleEnded_Start(0);
	delay(8);
	//Read Block 0
	ranges[0]=ad0.readADC_SingleEnded_Read(0);
	ranges[4] = ad0.readADC_SingleEnded_Read(0);
	ranges[8] = ad0.readADC_SingleEnded_Read(0);
	ranges[12] = ad0.readADC_SingleEnded_Read(0);

	//Start Reading Block 1:
	ad0.readADC_SingleEnded_Start(1);
	ad1.readADC_SingleEnded_Start(1);
	ad2.readADC_SingleEnded_Start(1);
	ad3.readADC_SingleEnded_Start(1);
	delay(8);
	//Read Block 1
	ranges[1] = ad0.readADC_SingleEnded_Read(1);
	ranges[5] = ad0.readADC_SingleEnded_Read(1);
	ranges[9] = ad0.readADC_SingleEnded_Read(1);
	ranges[13] = ad0.readADC_SingleEnded_Read(1);

	//Start Reading Block 2:
	ad0.readADC_SingleEnded_Start(2);
	ad1.readADC_SingleEnded_Start(2);
	ad2.readADC_SingleEnded_Start(2);
	ad3.readADC_SingleEnded_Start(2);
	delay(8);
	//Read Block 2
	ranges[2] = ad0.readADC_SingleEnded_Read(2);
	ranges[6] = ad0.readADC_SingleEnded_Read(2);
	ranges[10] = ad0.readADC_SingleEnded_Read(2);
	ranges[14] = ad0.readADC_SingleEnded_Read(2);

	//Start Reading Block 3:
	ad0.readADC_SingleEnded_Start(3);
	ad1.readADC_SingleEnded_Start(3);
	ad2.readADC_SingleEnded_Start(3);
	ad3.readADC_SingleEnded_Start(3);
	delay(8);
	//Read Block 3
	ranges[3] = ad0.readADC_SingleEnded_Read(3);
	ranges[7] = ad0.readADC_SingleEnded_Read(3);
	ranges[11] = ad0.readADC_SingleEnded_Read(3);
	ranges[15] = ad0.readADC_SingleEnded_Read(3);

}

//-------------------Loop--------------------

void loop() 
{
	#ifdef serialDebug

	measuretime = micros();

	fastread();
	//readallsensors();
	measuretime = micros() - measuretime;

	for (int i = 0; i <= 15; i++)
	{
		Serial.println(String("Sensor ")+(i)+(" : ")+(ranges[i]));
	}
	Serial.println(measuretime);
	Serial.println("------------------------------");


	#endif // serialDebug

	

	measurecount++;
	//delay(10);

}
