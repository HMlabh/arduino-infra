/*
 Name:		arduino-infra.ino
 Created:	07.02.2017 14:40:44
 Author:	gutek
*/


//Includes
#include <Wire.h>
#include <Adafruit_ADS1015.h>


//----------------------Defines----------------------------
//#define serialDebug //Debug output over Serial connection to PC
//#define serialDebug_continuous //reads the 16 Sensors continuous


//----------------------Params-----------------------------
namespace param 
{
	int8_t ident = 73;		//Identifier: 73 "I"
	int8_t ident_ask = 105; // define ident_ask Message: 105 "i"
	int8_t call = 99;		//define call Message: 99 "c"
	int8_t ask = 97;			// define ask Message: 97 "a"
}

int8_t mram = 0;	//Message storage

//----------------------Pinset-----------------------------

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
	int AD0 = 0x48;		//I2C Address of ADS 0
	int AD1 = 0x49;		//I2C Address of ADS 1
	int AD2 = 0x4A;		//I2C Address of ADS 2
	int AD3 = 0x4B;		//I2C Address of ADS 3
}

//----------------------Init-------------------------------

//Vector including all Enable-Pins
int enables[16] =
{ pin::EN0 , pin::EN1 , pin::EN2 , pin::EN3 , pin::EN4 , 
  pin::EN5 , pin::EN6 , pin::EN7 , pin::EN8 , pin::EN9 , 
  pin::EN10 , pin::EN11 , pin::EN12 , pin::EN13 , pin::EN14 , pin::EN15};

int16_t ranges[16] = { 0 };	//Rangevector ; Line = Sensornumber

int measurecount = 0;		//stores the number of measures done
long measuretime = 0;		//Storage variable for Time measurement


//-----ADC-----
Adafruit_ADS1015 ad0(addr::AD0);	// construct an ads1115 at address AD0
Adafruit_ADS1015 ad1(addr::AD1);	// construct an ads1115 at address AD1
Adafruit_ADS1015 ad2(addr::AD2);	// construct an ads1115 at address AD2
Adafruit_ADS1015 ad3(addr::AD3);	// construct an ads1115 at address AD3



//----------------------Setup------------------------------

void setup() 
{
	//starting serial connection to Host
	Serial.begin(115200);
	while (!Serial)
	{
		; // wait for serial port to connect. Needed for native USB port only
	}

#ifdef serialDebug	//sends debug message
		Serial.println("Serial Connection established");
		Serial.println(String("identifier : ")+(param::ident));
		Serial.println(String("call Message : ") + (param::call));
		Serial.println(String("ask Message : ") + (param::ask));
#endif // serialDebug

	//Pinmode Enables Output
	for (int i = 0; i <= 15; i++)
	{
		pinMode(enables[i], OUTPUT); //Sensor i Enable 0utput

#ifdef serialDebug
			Serial.println(String("Sensor ") + (i) + ("-> OUTPUT"));
#endif // serialDebug

	}
	
	//Begin serial communication to ADS1115
	ad0.begin();
	ad1.begin();
	ad2.begin();
	ad3.begin();

	//set Gain
	ad0.setGain(GAIN_TWOTHIRDS);
	ad1.setGain(GAIN_TWOTHIRDS);
	ad2.setGain(GAIN_TWOTHIRDS);
	ad3.setGain(GAIN_TWOTHIRDS);

	//activate all Sensors
	enableallsensors();
}


//----------------------Functions--------------------------

//enable [Sensor]
void enablesensor(int sensornr)
{
	digitalWrite(enables[sensornr], HIGH); //Sensor i Enable
}

//enable all Sensors
void enableallsensors()
{
	for (int i = 0; i <= 15; i++)
	{
		enablesensor(i); //Sensor i Enable
	}
	delay(100);//wait for Sensor to get ready
}

//disable [Sensor]
void disablesensor(int sensornr)
{
	digitalWrite(enables[sensornr], LOW); //Sensor i Disable
}

//disable all Sensors
void disableallsensors()
{
	for (int i = 0; i <= 15; i++)
	{
		disablesensor(i); //Sensor i Disable
	}
}

//read [Sensor]
int16_t readsensor(int sensornr)
{
	int16_t range = 0;//initialise return value

	if (sensornr >= 0 && sensornr <= 15)
	{
		switch (sensornr)//Lookup Sensor to measure
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

//read all Sensors in a row
void readallsensors()
{
	for (int i = 0; i <= 15; i++)
	{
		readsensor(i);
	}
}

//read all sensors 4 at a time
void fastread()
{
#ifdef serialDebug
		Serial.println("fastread");
#endif // serialDebug

	for (int i = 0; i <= 3; i++)
	{
		//Start Reading Block i:
		ad0.readADC_SingleEnded_Start(i);
		ad1.readADC_SingleEnded_Start(i);
		ad2.readADC_SingleEnded_Start(i);
		ad3.readADC_SingleEnded_Start(i);
		delayMicroseconds(100);
		//Read Block i
		ranges[0+i] = ad0.readADC_SingleEnded_Read(i);
		ranges[4+i] = ad1.readADC_SingleEnded_Read(i);
		ranges[8+i] = ad2.readADC_SingleEnded_Read(i);
		ranges[12+i] = ad3.readADC_SingleEnded_Read(i);
	}
}

//Send Solution vector to Node
void sendsolution()
{
#ifdef serialDebug
	Serial.println("Solution:");
#endif // serialDebug

	for (int i = 0; i <= 15; i++)
	{
		Serial.write(lowByte(ranges[i]));	//Send low 8-Bit of 16-Bit
		Serial.write(highByte(ranges[i]));	//Send high 8-Bit of 16-Bit
	}

#ifdef serialDebug
	Serial.println("");
	Serial.println("ASCII:");
	for (int i = 0; i <= 15; i++)
	{
		Serial.print(ranges[i]);
		Serial.print(" ; ");
	}
	Serial.println("");
#endif // serialDebug
}


//----------------------Loop-------------------------------

void loop() 
{

#ifdef serialDebug
	Serial.println("waiting...");
#endif // serialDebug

#ifndef serialDebug_continuous
	while (!Serial.available()) {}	//waits for a Message from Node
	mram = Serial.read();			//reads Message from Node
#endif //serialDebug_continuous

#ifdef serialDebug
	Serial.println("message received");
#endif // serialDebug

	
	if (mram == param::call)//call message received
	{
#ifdef serialDebug
		Serial.println(String("..call message.."));
#endif // serialDebug

		fastread();
	}
//---------------------------------------------------------
	else if (mram == param::ask)//ask message recieved
	{
#ifdef serialDebug
		Serial.println(String("..ask message.."));
#endif // serialDebug

		sendsolution();
	}
//---------------------------------------------------------
	else if (mram == param::ident_ask)//ident_ask message recieved
	{
#ifdef serialDebug
		Serial.println(String("Ident:"));
#endif // serialDebug

		Serial.write(param::ident);

#ifdef serialDebug
		Serial.println(String(" "));
#endif // serialDebug
	}
//---------------------------------------------------------
	else//anny other message -> do nothing
	{
#ifdef serialDebug
		Serial.println(String("!!unidentified message!!"));
#endif // serialDebug
	}


#ifdef serialDebug_continuous
	measuretime = micros();
	fastread();
	measuretime = micros() - measuretime;
	for (int i = 0; i <= 15; i++)
	{
		Serial.println(String("Sensor ") + (i)+(" : ") + (ranges[i]));
	}
	Serial.println(String(measuretime) + ("  |  ") + (measurecount));

	Serial.println("------------------------------");
	measurecount++;
	delay(200);
#endif // serialDebug_continuous

}
