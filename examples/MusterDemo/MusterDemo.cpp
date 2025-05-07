/**
*   @file   MusterDemo.cpp
*   @author Benjamin Marty (bmarty@kochag.ch)
*   @date   17.12.2014
*   @date   29.04.2025  (New project version)
*   @brief  Main File of MusterDemo Example
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
  magentaobj.read_io();
  
	//Speicher von MagentaCore leeren
	magentaobj.clear();

	//1s warten
	delay(1000);
	
	//MagentaCore Muster 1 in Speicher schreiben
	magentaobj.sample(1);

	//1s warten
	delay(1000);
}