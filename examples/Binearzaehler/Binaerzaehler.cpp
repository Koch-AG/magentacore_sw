/**
*   @file   Binearzaehler.cpp
*   @author Benjamin Marty (bmarty@kochag.ch)
*   @date  	14.12.2014
*	@date	14.01.2015
*	@date	29.04.2025	(New project version)
*   @brief  Main File of Binaerzaehler Example
*   @bug    No known bugs.
*/

#include <Magenta_lib.h>

//Zahlvariabel fuer innerhalb vom Main
int x = 0;

void setup()
{
	//Initalisierung der MagentaCore HW Komponenten
	magentaobj.init();


}

void loop()
{
	magentaobj.read_io();

	//Wenn x 255 entspricht
	if(x == 255)
	{
		//x auf null setzen da 8Bit = maximal Dezimal 255
		x = 0;
	}
	else
	{
		//x inkrementieren
		x++;
	}
	
	//oberste Zeile entsprechend x in Binaer ausgeben, restliche Zeilen 0
	magentaobj.write(x, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);   
    
	//200ms warten
	delay(200);
}