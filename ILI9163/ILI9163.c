#include "ILI9163.h"

#include <wiringPi.h>
#include <wiringPiSPI.h>

#define BCMLIB 1			// BCMLIB1

#ifdef   BCMLIB
#include <bcm2835.h>
#endif




TFT::TFT ( int cs, int rst,int dc ) : 
	GFX ( WIDTH, HEIGHT )
{
	csPin  = cs;
	rstPin = rst;
	dcPin  = dc;
}

void TFT::drawPixel( int16_t x, int16_t y, uint16_t color )
{	
	if ((x >= 0) && (x < WIDTH) && (y >= 0) && (y < HEIGHT))
	{
		displayMap[y][x] |= color;
	}
}

void TFT::drawFastVLine ( int16_t x, int16_t y, int16_t h, uint16_t color) {
	
	if ((x >= 0) && (x < WIDTH) && (y >= 0) && ( (y+h) <= HEIGHT))  // would be better
	for ( int i = 0; i < h; i++ ) {
		displayMap[y+i][x] |= color;
	}	
}

void TFT::drawFastHLine ( int16_t x, int16_t y, int16_t w, uint16_t color) {
	if ((x >= 0) && (x+w <= WIDTH) && (y >= 0) && ( y < HEIGHT))   // wold be better
	for ( int i=0; i < w; i++ ) {
		displayMap[y][x+i] |= color;
	}	
}


//
void TFT::setAddrWindow(unsigned short  x0, unsigned short  y0, unsigned short  x1, unsigned short  y1) {

	writecommand(CMD_CLMADRS); // Column
	if (rotation == 0 || rotation > 1){
		writedata16(x0);
		writedata16(x1);
	} else {
		writedata16(x0 + __OFFSET);
		writedata16(x1 + __OFFSET);
	}
	writecommand(CMD_PGEADRS); // Page
	if (rotation == 0){
		writedata16(y0 + __OFFSET);
		writedata16(y1 + __OFFSET);
	} else {
		writedata16(y0);
		writedata16(y1);
	}
	writecommand(CMD_RAMWR); //Into RAM
}

//
void TFT::setRotation( unsigned char m) {
	rotation = m % 4; // can't be higher than 3
	switch (rotation) {
	case 0:
		_Mactrl_Data = 0b00001000;
		_width  = _TFTWIDTH;
		_height = _TFTHEIGHT;//-__OFFSET;
		break;
	case 1:
		_Mactrl_Data = 0b01101000;
		_width  = _TFTHEIGHT;//-__OFFSET;
		_height = _TFTWIDTH;
		break;
	case 2:
		_Mactrl_Data = 0b11001000;
		_width  = _TFTWIDTH;
		_height = _TFTHEIGHT;//-__OFFSET;
		break;
	case 3:
		_Mactrl_Data = 0b10101000;
		_width  = _TFTWIDTH;
		_height = _TFTHEIGHT;//-__OFFSET;
		break;
	}
	colorSpace(_colorspaceData);
}

void TFT::colorSpace( unsigned char cspace) {
	if (cspace < 1)
		_Mactrl_Data &= 0b11110111;
	else
		_Mactrl_Data |= 0b00001000;
}



 
// clear framebuffer
void TFT::clearDisplay()
{	
	unsigned short *p = & displayMap[0][0];
	for (int i=0; i<( WIDTH * HEIGHT); i++)
    {
      *p = random () % 65535;
	   p++;
    }
}





// init TFT board	
void TFT::init() {
	
	initSpi();		

//	unsigned char _colorspaceData = __COLORSPC;			//start with default data;
	
	writecommand(CMD_SWRESET);//software reset
	delay(100);
		
	writecommand(CMD_SLPOUT);//exit sleep
	delay(5);
		
	writecommand(CMD_PIXFMT);//Set Color Format 16bit   
	writedata(0x05);
	delay(5);
		
	writecommand(CMD_GAMMASET);//default gamma curve 3
	writedata(0x04);//0x04
	delay(1);
		
	writecommand(CMD_GAMRSEL);//Enable Gamma adj    
	writedata(0x01); 
	delay(1);
		
	writecommand(CMD_NORML);
	
	writecommand(CMD_DFUNCTR);
	writedata(0b11111111);		//
	writedata(0b00000110);		//

	writecommand(CMD_PGAMMAC);	//Positive Gamma Correction Setting
	for (int i=0;i<15;i++){
		writedata(pGammaSet[i]);
	}
		
	writecommand(CMD_NGAMMAC);	//Negative Gamma Correction Setting
	for (int i=0;i<15;i++){
		writedata(nGammaSet[i]);
	}

	writecommand(CMD_FRMCTR1);  //Frame Rate Control (In normal mode/Full colors)
	writedata(0x08);			//0x0C//0x08
	writedata(0x02);	        //0x14//0x08
	delay(1);
		
	writecommand(CMD_DINVCTR);//display inversion 
	writedata(0x07);
	delay(1);
		
	writecommand(CMD_PWCTR1);//Set VRH1[4:0] & VC[2:0] for VCI1 & GVDD   
	writedata(0x0A);//4.30 - 0x0A
	writedata(0x02);//0x05
	delay(1);
		
	writecommand(CMD_PWCTR2);//Set BT[2:0] for AVDD & VCL & VGH & VGL   
	writedata(0x02);
	delay(1);
		
	writecommand(CMD_VCOMCTR1);//Set VMH[6:0] & VML[6:0] for VOMH & VCOML   
	writedata(0x50);//0x50
	writedata(99);//0x5b
	delay(1);
		
	writecommand(CMD_VCOMOFFS);
	writedata(0);//0x40
	delay(1);
  
	writecommand(CMD_CLMADRS);//Set Column Address  
	writedata16(0x00); 
	writedata16(_GRAMWIDTH); 
  
	writecommand(CMD_PGEADRS);//Set Page Address  
	writedata16(0X00); 
	writedata16(_GRAMHEIGH);
	
	
	// set scroll area (thanks Masuda)
	writecommand(CMD_VSCLLDEF);
	writedata16(__OFFSET);
	writedata16(_GRAMHEIGH - __OFFSET);
	writedata16(0);
		
	//colorSpace(_colorspaceData);
	// MY, MX, MV, ML,RGB, MH, D1, D0
	// 0 | 0 | 0 | 0 | 1 | 0 | 0 | 0	//normal
	// 1 | 0 | 0 | 0 | 1 | 0 | 0 | 0	//Y-Mirror
	// 0 | 1 | 0 | 0 | 1 | 0 | 0 | 0	//X-Mirror
	// 1 | 1 | 0 | 0 | 1 | 0 | 0 | 0	//X-Y-Mirror
	// 0 | 0 | 1 | 0 | 1 | 0 | 0 | 0	//X-Y Exchange
	// 1 | 0 | 1 | 0 | 1 | 0 | 0 | 0	//X-Y Exchange, Y-Mirror
	// 0 | 1 | 1 | 0 | 1 | 0 | 0 | 0	//XY exchange
	// 1 | 1 | 1 | 0 | 1 | 0 | 0 | 0

	_Mactrl_Data = 0b11000000;
	_colorspaceData = __COLORSPC;    //start with default data;
	
	colorSpace(_colorspaceData);
	
	rotation = 0;
	writecommand(CMD_MADCTL);
	writedata(_Mactrl_Data);	
    writecommand(CMD_DISPON);//display ON 
	delay(1);
	writecommand(CMD_RAMWR);//Memory Write

	delay(1);

}

#ifndef BCMLIB

void TFT::initSpi() {
			
	wiringPiSetup (); 
	int v = wiringPiSPISetup (1, 16000000) ;

	if ( v != -1 )
		printf(" Openned SPI succesfully \n" ); 
	else 
		printf(" SPI not openned !\n" ); 
	
	//Configure control pins
    pinMode(csPin  , OUTPUT);
	pinMode(rstPin , OUTPUT);
	pinMode(dcPin  , OUTPUT);
	
	digitalWrite ( csPin, LOW );
    digitalWrite ( rstPin, HIGH);
	delay(50);
	digitalWrite ( rstPin, LOW);
	delay(50);
	digitalWrite ( rstPin, HIGH);
	delay(50);
}

//**********************************************************************
void TFT::spiWrite(unsigned char data)
{
   digitalWrite(csPin, LOW);
   wiringPiSPIDataRW ( 1 , &data , 1) ; //Send the data
   digitalWrite(csPin, HIGH);
}
//**********************************************************************
void TFT::writecommand(unsigned char c)
{
	digitalWrite ( dcPin, LOW );  // low
	spiWrite(c);
}
//**********************************************************************
void TFT::writedata( unsigned char c)
{
	digitalWrite ( dcPin, HIGH );  // data
	spiWrite(c);
} 
//**********************************************************************
void TFT::writedata16( unsigned short int d)
{
	digitalWrite ( dcPin, HIGH );  // data
    spiWrite(d >> 8);			   // msb first
	spiWrite(d);				   // lsb secund
} 
//**********************************************************************
void TFT::pushdata16( unsigned short int d)
{
  wiringPiSPIDataRW ( 1 , ( unsigned char* ) &d , 2) ; //Send the data
} 
//**********************************************************************
// Send whole frame to TFT GRAM
void TFT::updateDisplay() {
	
	setAddrWindow (0x00,0x00, _GRAMWIDTH, _GRAMHEIGH );	//go home
    digitalWrite ( dcPin, HIGH );  // data
	digitalWrite ( csPin, LOW );
    for (int y = 0; y < HEIGHT; y++){
		wiringPiSPIDataRW ( 1 , ( unsigned char* ) &displayMap[y][0] , 2* WIDTH );
	}
	setAddrWindow (0x00,0x00, _GRAMWIDTH, _GRAMHEIGH );	//go home
	digitalWrite ( csPin, HIGH );

}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
#else

void TFT::initSpi() {

	if (bcm2835_init() )
		printf(" BCM2835 LIB initiated! \n\n" ); 
	else 
		printf(" BCM2853 LIB initiation error !\n" ); 

	printf ( "PINS2: %d , %d, %d \n", csPin, rstPin, dcPin );
	
	if ( bcm2835_spi_begin() )
		printf(" SPI oppened \n" ); 
	else
		printf (" SPI is not oppened!" );
		
	//Set CS pins polarity to low
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, 0);
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, 0);
	//	BCM2835_SPI_CLOCK_DIVIDER_1     = 1,       ///< 1 = 262.144us = 3.814697260kHz, same as 0/65536
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_8);
	//Set SPI data mode
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);		//(SPI_MODE_# | SPI_CS_HIGH)=Chip Select active high, (SPI_MODE_# | SPI_NO_CS)=1 device per bus no Chip Select

	//Set with CS pin to use for next transfers
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	
	//Configure control pins
	bcm2835_gpio_fsel(csPin , BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(rstPin, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(dcPin , BCM2835_GPIO_FSEL_OUTP);

	bcm2835_gpio_write( csPin, LOW );
	bcm2835_gpio_write( rstPin, HIGH);
	delay(50);
	bcm2835_gpio_write( rstPin, LOW);
	delay(50);
	bcm2835_gpio_write( rstPin, HIGH);
	delay(50);
	

	//Return SPI pins to default inputs state
	//bcm2835_spi_end();
}



// Send whole frame to TFT GRAM
void TFT::updateDisplay() {
	setAddrWindow (0x00,0x00, _GRAMWIDTH, _GRAMHEIGH );	//go home
	bcm2835_gpio_write(dcPin, HIGH);
	bcm2835_gpio_write(csPin, LOW);

    for (int y = 0; y < HEIGHT; y++){
		bcm2835_spi_transfern(( char* ) &displayMap[y][0] , 2* WIDTH);
	}
	setAddrWindow (0x00,0x00, _GRAMWIDTH, _GRAMHEIGH );	//go home
	bcm2835_gpio_write(csPin, HIGH);

}

void TFT::spiWrite(unsigned char data)
{
	bcm2835_gpio_write(csPin, LOW);
    data = bcm2835_spi_transfer( data );			//Send the data   
	bcm2835_gpio_write(csPin, HIGH);
}
//**********************************************************************
void TFT::writecommand(unsigned char c)
{
	bcm2835_gpio_write( dcPin, LOW ); // command
	spiWrite(c);
}
//**********************************************************************
void TFT::writedata( unsigned char c)
{
	bcm2835_gpio_write( dcPin, HIGH );  // data
	spiWrite(c);
} 
//**********************************************************************
void TFT::writedata16( unsigned short int d)
{
	bcm2835_gpio_write (dcPin, HIGH ); // data
    spiWrite(d >> 8);				   // msb first
	spiWrite(d);				   	   // lsb secund
} 

void TFT::pushdata16( unsigned short int d)
{
  //wiringPiSPIDataRW ( 1 , ( unsigned char* ) &d , 2) ; //Send the data
} 

#endif




