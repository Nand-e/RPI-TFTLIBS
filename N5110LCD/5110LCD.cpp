

/* PCD8544-specific defines: */
#define LCD_COMMAND  0
#define LCD_DATA     1

#include "5110LCD.h"

#include <wiringPi.h>
#include <wiringPiSPI.h>

#include <stdio.h>
#include <stdlib.h>


NOKIALCD::NOKIALCD ( int dc, int sce, int rst ) : 
	GFX ( LCD_WIDTH, LCD_HEIGHT )
{
	scePin = sce;
	rstPin = rst;
	dcPin  = dc;
}

// This function sets a pixel on displayMap to your preferred
// color. 1=Black, 0= white.
void NOKIALCD::drawPixel( int16_t x, int16_t y, uint16_t color )
{
  // First, double check that the coordinate is in range.
  if ((x >= 0) && (x < LCD_WIDTH) && (y >= 0) && (y < LCD_HEIGHT))
  {
    unsigned char shift = y % 8;

    if ( color > 0 ) // If black, set the bit.
      displayMap[x + (y/8)*LCD_WIDTH] |= 1<<shift;
    else   // If white clear the bit.
      displayMap[x + (y/8)*LCD_WIDTH] &= ~(1<<shift);
  }
}

// There are two memory banks in the LCD, data/RAM and commands.
// This function sets the DC pin high or low depending, and then
// sends the data byte
void NOKIALCD::LCDWrite( unsigned char data_or_command, unsigned char data)
{
   //Tell the LCD that we are writing either to data or a command
   digitalWrite(dcPin, data_or_command);

   //Send the data
   digitalWrite(scePin, LOW);
   wiringPiSPIDataRW ( 1 , &data , 1) ;
   digitalWrite(scePin, HIGH);
}


// This will actually draw on the display, whatever is currently
// in the displayMap array.
void NOKIALCD::updateDisplay() {

   LCDWrite(0, 0x80 | 0);  // Column.
   LCDWrite(0, 0x40 | 0);  // Row.  ?

   //Tell the LCD that we are writing either to data or a command
   digitalWrite(dcPin, LCD_DATA);
   //Send the data
   digitalWrite(scePin, LOW);
   wiringPiSPIDataRW ( 1 , displayMap , (LCD_WIDTH * LCD_HEIGHT / 8) ) ;
   digitalWrite(scePin, HIGH);
   
   /*gotoXY(0, 0);
   for (int i=0; i < (LCD_WIDTH * LCD_HEIGHT / 8); i++)
   {
     LCDWrite(LCD_DATA, displayMap[i]);
   }
   */
 }

void NOKIALCD::clearDisplay(bool bw)
{
  for (int i=0; i<(LCD_WIDTH * LCD_HEIGHT / 8); i++)
  {
	 displayMap[i] =0;
  }
}

// Set contrast can set the LCD Vop to a value between 0 and 127.
// 40-60 is usually a pretty good range.
void NOKIALCD::setContrast( unsigned char contrast)
{
  LCDWrite(LCD_COMMAND, 0x21); 				//Tell LCD that extended commands follow
  LCDWrite(LCD_COMMAND, 0x80 | contrast);   //Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
  LCDWrite(LCD_COMMAND, 0x20); 				//Set display mode
}

	
void NOKIALCD::init() {
	
	wiringPiSetup (); 
	int v = wiringPiSPISetup (1, 4000000) ;

	if ( v != -1 ) {
		printf(" Openned SPI succesfully \n" ); 
	}
	else {
		printf(" SPI not openned !\n" ); 
	}
	  //Configure control pins
    pinMode(scePin, OUTPUT);
	pinMode(rstPin, OUTPUT);
	pinMode(dcPin, OUTPUT);

   //Reset the LCD to a known state
    digitalWrite(rstPin, LOW);
    digitalWrite(rstPin, HIGH);

	LCDWrite(LCD_COMMAND, 0x21); //Tell LCD extended commands follow
	LCDWrite(LCD_COMMAND, 0xB0); //Set LCD Vop (Contrast)
	LCDWrite(LCD_COMMAND, 0x04); //Set Temp coefficent
	LCDWrite(LCD_COMMAND, 0x14); //LCD bias mode 1:48 (try 0x13)
	//We must send 0x20 before modifying the display control mode
	LCDWrite(LCD_COMMAND, 0x20);
	LCDWrite(LCD_COMMAND, 0x0C); //Set display control, normal mode.
}

