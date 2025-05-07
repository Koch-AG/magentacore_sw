/**
*   @file   EEinAusDemo.cpp
*   @author Benjamin Marty (bmarty@kochag.ch)
*   @date   17.12.2014
*   @date   29.04.2025 (New project version)
*   @brief  Main File of EinAusDemo Example
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
	
	//Die 8 angegebenen Bytes in MagentaCore Speicher übernehmen  
	magentaobj.write(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
  
	//1s warten	
	delay(1000);
}
