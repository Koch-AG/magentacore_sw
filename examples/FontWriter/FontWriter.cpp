/**
*   @file   FontWriter.cpp
*   @author Benjamin Marty (bmarty@kochag.ch)
*   @date   11.8.2015
*   @date   29.04.2025  (New project version)
*   @brief  Main File of FontWriter Example
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
	
  for(int x = 64; x <= 90; x++)
  {
     magentaobj.write_char(x);       
     delay(1000);
  }
        
  magentaobj.write_char(142);   
  delay(1000);
        
  magentaobj.write_char(153);
  delay(1000);
        
  magentaobj.write_char(154);  
  delay(1000);
}
