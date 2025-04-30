/**
*   @file   KeyPotiTestSerial.cpp
*   @author Benjamin Marty (bmarty@kochag.ch)
*   @date   03.03.2014
*   @date   29.04.2025  (New project version)
*   @brief  Main File of KeyPotiTestSerial Example
*   @bug    No known bugs.
*
*/

#include <Magenta_lib.h>

//Instanz der Klasse bilden
//MagentaCore magentaobj;

int potentiometer_vorher;

char bla[8] = { 0b10101010, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0b01010101 };

void setup()
{
	//Initalisierung der MagntaCore HW Komponenten
	magentaobj.init();
	
	//attachInterrupt(0, taster, FALLING);

	Serial.begin(9600);	

	magentaobj.write_array((byte*)bla);
}


void loop()
{
	magentaobj.read_io();

	if(magentaobj.button_1)
	{
		printf("Taste1\n");
	}

	if(magentaobj.button_2)
	{
		printf("Taste2\n");
	}

	if(magentaobj.button_3)
	{
		printf("Taste3\n");

	}

	if(magentaobj.potentiometer_0 != potentiometer_vorher)
	{
		printf("Potentiometer aendert sich\n");
		potentiometer_vorher = magentaobj.potentiometer_0;
	}

}