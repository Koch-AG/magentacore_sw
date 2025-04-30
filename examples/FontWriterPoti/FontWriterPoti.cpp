/**
*   @file   FontWriterPoti.cpp
*   @author Benjamin Marty (bmarty@kochag.ch)
*   @date   11.8.2015
*   @date   29.04.2025  (New project version)
*   @brief  Main File of FontWriterPoti Example
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

	magentaobj.write_char(magentaobj.potentiometer_0/40+65);

	delay(20);
}
