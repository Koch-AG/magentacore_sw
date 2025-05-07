/**
*   @file   FontWriterKey.cpp
*   @author Benjamin Marty (bmarty@kochag.ch)
*   @date   12.04.2016
*   @date   29.04.2025  (New project version)
*   @brief  Main File of FontWriterKey Example
*   @bug    No known bugs.
*
*/

#include <Magenta_lib.h>

void setup()
{
	//Initalisierung der MagentaCore HW Komponenten
	magentaobj.init();
}


void loop()
{
	//Eingaenge (Potentiometer + 3Taster abfragen und aktualisieren)
	magentaobj.read_io();

	//Pruefen ob Taste 1 gedrueckt
	if(magentaobj.button_1)
	{
		//Ascii Zeichen ausgeben
		magentaobj.write_char(49);
	}
	
	//Pruefen ob Taste 2 gedrueckt
	if(magentaobj.button_2)
	{
		//Ascii Zeichen ausgeben
		magentaobj.write_char(50);
	}

	//Pruefen ob Taste 3 gedrueckt
	if(magentaobj.button_3)
	{
		//Ascii Zeichen ausgeben
		magentaobj.write_char(51);
	}
}