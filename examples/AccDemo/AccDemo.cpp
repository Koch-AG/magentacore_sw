/**
 *  @file   AccDemo.cpp
 *  @author Benjamin Marty (bmarty@kochag.ch)
 *  @date   17.12.2014
 *  @date	29.04.2025	(New project version)
 *  @brief	Main File of AccDemo Example
 *  @bug	No known bugs.
 */


 /*
 This example shows how to control and 
 move a single LED using the accelerometer.
 When you move the PCB, the LED moves.
 */

#include <Magenta_lib.h>

//Variable for the left and right move
int move = 0;

void setup()
{
	//Initalisierung der MagentaCore HW Komponenten
  magentaobj.init();
}

void loop()
{
  	magentaobj.read_io();

	move = ((magentaobj.accelerometer_x + 1) * 4);
	
	if((magentaobj.accelerometer_y >= 0.8) && (magentaobj.accelerometer_y <= 0.95))
	{
		magentaobj.write((0x80 >> move), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
	}
	if((magentaobj.accelerometer_y >= 0.5) && (magentaobj.accelerometer_y <= 0.7))
	{
		magentaobj.write(0x00, (0x80 >> move), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
	}
	if((magentaobj.accelerometer_y >= 0.2) && (magentaobj.accelerometer_y <= 0.4))
	{
		magentaobj.write(0x00, 0x00, (0x80 >> move), 0x00, 0x00, 0x00, 0x00, 0x00);
	}
	if((magentaobj.accelerometer_y >= 0) && (magentaobj.accelerometer_y <= 0.2))
	{
		magentaobj.write(0x00, 0x00, 0x00, (0x80 >> move), 0x00, 0x00, 0x00, 0x00);
	}
       
	if((magentaobj.accelerometer_y >= -0.2) && (magentaobj.accelerometer_y <= -0))
	{
		magentaobj.write(0x00, 0x00, 0x00, 0x00, (0x80 >> move), 0x00, 0x00, 0x00);
	}
	if((magentaobj.accelerometer_y >=  -0.4) && (magentaobj.accelerometer_y <= -0.2))
	{
		magentaobj.write(0x00, 0x0, 0x00, 0x00, 0x00, (0x80 >> move), 0x00, 0x00);
	}
	if((magentaobj.accelerometer_y >= -0.7) && (magentaobj.accelerometer_y <= -0.5))
	{
		magentaobj.write(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (0x80 >> move), 0x00);
	}
	if((magentaobj.accelerometer_y >= -0.95) && (magentaobj.accelerometer_y <= -0.8))
	{
		magentaobj.write(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (0x80 >> move));
	}         
	    
	//10ms warten
	delay(10);
}
