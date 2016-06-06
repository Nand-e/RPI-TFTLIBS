

#include "ILI9341.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include <sched.h>
#include <sys/mman.h>
#include <string.h>

#define BCMLIB 1

#ifdef   BCMLIB
#include <bcm2835.h>
#endif

#define ILI9341_TFTWIDTH  240
#define ILI9341_TFTHEIGHT 320

#define ILI9341_NOP     0x00
#define ILI9341_SWRESET 0x01
#define ILI9341_RDDID   0x04
#define ILI9341_RDDST   0x09

#define ILI9341_SLPIN   0x10
#define ILI9341_SLPOUT  0x11
#define ILI9341_PTLON   0x12
#define ILI9341_NORON   0x13

#define ILI9341_RDMODE  0x0A
#define ILI9341_RDMADCTL  0x0B
#define ILI9341_RDPIXFMT  0x0C
#define ILI9341_RDIMGFMT  0x0D
#define ILI9341_RDSELFDIAG  0x0F
#define ILI9341_INVOFF  0x20
#define ILI9341_INVON   0x21
#define ILI9341_GAMMASET 0x26
#define ILI9341_DISPOFF 0x28
#define ILI9341_DISPON  0x29
#define ILI9341_CASET   0x2A
#define ILI9341_PASET   0x2B
#define ILI9341_RAMWR   0x2C
#define ILI9341_RAMRD   0x2E

#define ILI9341_PTLAR   0x30
#define ILI9341_MADCTL  0x36
#define ILI9341_PIXFMT  0x3A
#define ILI9341_FRMCTR1 0xB1
#define ILI9341_FRMCTR2 0xB2
#define ILI9341_FRMCTR3 0xB3
#define ILI9341_INVCTR  0xB4
#define ILI9341_DFUNCTR 0xB6

#define ILI9341_PWCTR1  0xC0
#define ILI9341_PWCTR2  0xC1
#define ILI9341_PWCTR3  0xC2
#define ILI9341_PWCTR4  0xC3
#define ILI9341_PWCTR5  0xC4
#define ILI9341_VMCTR1  0xC5
#define ILI9341_VMCTR2  0xC7
#define ILI9341_RDID1   0xDA
#define ILI9341_RDID2   0xDB
#define ILI9341_RDID3   0xDC
#define ILI9341_RDID4   0xDD
#define ILI9341_GMCTRP1 0xE0
#define ILI9341_GMCTRN1 0xE1

//**********************************************************************
TFT::TFT ( int dc, int reset, int cs ) :
	GFX ( 320, 240 )
 {
	dcPin  = dc;
	rstPin = reset;
	csPin  = cs;
}

void TFT::drawPixel(int16_t x, int16_t y, uint16_t color) {
	displayMap [x][y] |= color;
}



void TFT::init () {
	initSpi();
	
	
	struct sched_param sp;
	memset(&sp, 0, sizeof(sp));
	sp.sched_priority = sched_get_priority_max(SCHED_FIFO);
	sched_setscheduler(0, SCHED_FIFO, &sp);
	mlockall(MCL_CURRENT | MCL_FUTURE);
		
  //  SPI.setBitOrder(MSBFIRST);
  //  SPI.setDataMode(SPI_MODE0);
	
  // digitalWrite( RESET, HIGH); delay(5);
  // digitalWrite( RESET, LOW);  delay(20);
  // digitalWrite( RESET, HIGH); delay(150);
  
	writeCommand(0xEF);
	writeData(0x03);
	writeData(0x80);
	writeData(0x02);

	writeCommand(0xCF);  
	writeData(0x00); 
	writeData(0XC1); 
	writeData(0X30); 

	writeCommand(0xED);  
	writeData(0x64); 
	writeData(0x03); 
	writeData(0X12); 
	writeData(0X81); 
 
	writeCommand(0xE8);  
	writeData(0x85); 
	writeData(0x00); 
	writeData(0x78); 

	writeCommand(0xCB);  
	writeData(0x39); 
	writeData(0x2C); 
	writeData(0x00); 
	writeData(0x34); 
	writeData(0x02); 
 
	writeCommand(0xF7);  
	writeData(0x20); 

	writeCommand(0xEA);  
	writeData(0x00); 
	writeData(0x00); 
	 
	writeCommand(ILI9341_PWCTR1);    //Power control 
	writeData(0x23);   //VRH[5:0] 
	 
	writeCommand(ILI9341_PWCTR2);    //Power control 
	writeData(0x10);   //SAP[2:0];BT[3:0] 
	 
	writeCommand(ILI9341_VMCTR1);    //VCM control 
	writeData(0x3e); 				   //¶
	writeData(0x28); 
	  
	writeCommand(ILI9341_VMCTR2);    //VCM control2 
	writeData(0x86);                  //--
	 
	writeCommand(ILI9341_MADCTL);    // Memory Access Control 
	writeData(0x48);

	writeCommand(ILI9341_PIXFMT);    
	writeData(0x55); 
	  
	writeCommand(ILI9341_FRMCTR1);    
	writeData(0x00);  
	writeData(0x18); 
	 
	writeCommand(ILI9341_DFUNCTR);    // Display Function Control 
	writeData(0x08); 
	writeData(0x82);
	writeData(0x27);  
	 
	writeCommand(0xF2);    // 3Gamma Function Disable 
	writeData(0x00); 
	 
	writeCommand(ILI9341_GAMMASET);    //Gamma curve selected 
	writeData(0x01); 
	 
	writeCommand(ILI9341_GMCTRP1);    //Set Gamma 
	writeData(0x0F); 
	writeData(0x31); 
	writeData(0x2B); 
	writeData(0x0C); 
	writeData(0x0E); 
	writeData(0x08); 
	writeData(0x4E); 
	writeData(0xF1); 
	writeData(0x37); 
	writeData(0x07); 
	writeData(0x10); 
	writeData(0x03); 
	writeData(0x0E); 
	writeData(0x09); 
	writeData(0x00); 
	  
    writeCommand(ILI9341_GMCTRN1);    //Set Gamma 
    writeData(0x00); 
    writeData(0x0E); 
    writeData(0x14); 
    writeData(0x03); 
    writeData(0x11); 
    writeData(0x07); 
	writeData(0x31); 
    writeData(0xC1); 
    writeData(0x48); 
	writeData(0x08); 
    writeData(0x0F); 
    writeData(0x0C); 
    writeData(0x31); 
    writeData(0x36); 
    writeData(0x0F);     
    
    writeCommand(ILI9341_SLPOUT);    //Exit Sleep 
    delay(120); 	
    writeCommand(ILI9341_DISPON);    //Display on 
}

void TFT::sendLine ( unsigned short int x) {
	digitalWrite( dcPin, HIGH);
    digitalWrite( csPin, LOW);
    wiringPiSPIDataRW ( 1 , ( unsigned char * ) &displayMap[x][0],480) ;
    digitalWrite( csPin, HIGH);
	
}


void TFT::drawFastVLine(int16_t x, int16_t y,int16_t h, uint16_t color) {
  // Update in subclasses if desired!
  for ( int i = y; i < y +h; i ++ ) {
	  displayMap[x][i] |= color;
  }
 }

void TFT::drawFastHLine(int16_t x, int16_t y,int16_t w, uint16_t color) {
  // Update in subclasses if desired!
  for ( int i = x; i < x +w; i ++ ) {
	  displayMap[i][y] |= color;
  }
 }

void TFT::setAddrWindow( short int x0, short int y0, short  int x1, short int y1) {

  writeCommand(ILI9341_CASET); // Column addr set
  writeData(x0 >> 8);
  writeData(x0 & 0xFF);     // XSTART 
  writeData(x1 >> 8);
  writeData(x1 & 0xFF);     // XEND

  writeCommand(ILI9341_PASET); // Row addr set
  writeData(y0>>8);
  writeData(y0);     // YSTART
  writeData(y1>>8);
  writeData(y1);     // YEND

  writeCommand(ILI9341_RAMWR); // write to RAM
}

void TFT::pushColor ( unsigned short int color ) {

  digitalWrite(dcPin, HIGH);
  digitalWrite(csPin, LOW);

  spiWrite(color >> 8);
  spiWrite(color);

  digitalWrite( csPin, HIGH);

}

#ifndef BCMLIB

void TFT::initSpi() {
			
	wiringPiSetup (); 
	int v = wiringPiSPISetup (1, 64000000) ;

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
void TFT::writeCommand(unsigned char c)
{
	digitalWrite ( dcPin, LOW );  // low
	spiWrite(c);
}
//**********************************************************************
void TFT::writeData( unsigned char c)
{
	digitalWrite ( dcPin, HIGH );  // data
	spiWrite(c);
} 
//**********************************************************************
void TFT::writeData16( unsigned short int d)
{
	digitalWrite ( dcPin, HIGH );  // data
    spiWrite(d >> 8);			   // msb first
	spiWrite(d);				   // lsb secund
} 
//**********************************************************************
void TFT::pushData16( unsigned short int d)
{
  wiringPiSPIDataRW ( 1 , ( unsigned char* ) &d , 2) ; //Send the data
} 
//**********************************************************************
// Send whole frame to TFT GRAM
void TFT::updateDisplay() {
	
	setAddrWindow (0x00,0x00, HEIGHT, WIDTH );	//go home
    digitalWrite ( dcPin, HIGH );  // data
	digitalWrite ( csPin, LOW );
    for (int y = 0; y <WIDTH; y++){
		wiringPiSPIDataRW ( 1 , ( unsigned char* ) &displayMap[y][0] , 2* HEIGHT );
	}
	setAddrWindow (0x00,0x00, HEIGHT, WIDTH );	//go home
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
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_4);
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
	delay(150);
	

	//Return SPI pins to default inputs state
	//bcm2835_spi_end();
}


//**********************************************************************
// Send whole frame to TFT GRAM
void TFT::updateDisplay() {
	setAddrWindow (0x00,0x00, HEIGHT, WIDTH);	//go home
	bcm2835_gpio_write(dcPin, HIGH);
	bcm2835_gpio_write(csPin, LOW);
	//  for (int y = 0; y < WIDTH; y++) { 
		bcm2835_spi_transfern(( char* ) &displayMap[0][0] , 2* HEIGHT * WIDTH);
    //}
	setAddrWindow (0x00,0x00, HEIGHT, WIDTH);	//go home
	bcm2835_gpio_write(csPin, HIGH);

}
//**********************************************************************
void TFT::spiWrite(unsigned char data)
{
	bcm2835_gpio_write(csPin, LOW);
    data = bcm2835_spi_transfer( data );			//Send the data   
	bcm2835_gpio_write(csPin, HIGH);
}
//**********************************************************************
void TFT::writeCommand(unsigned char c)
{
	bcm2835_gpio_write( dcPin, LOW ); // command
	spiWrite(c);
}
//**********************************************************************
void TFT::writeData( unsigned char c)
{
	bcm2835_gpio_write( dcPin, HIGH );  // data
	spiWrite(c);
} 
//**********************************************************************
void TFT::writeData16( unsigned short int d)
{
	bcm2835_gpio_write (dcPin, HIGH ); // data
    spiWrite(d >> 8);				   // msb first
	spiWrite(d);				   	   // lsb secund
} 

void TFT::pushData16( unsigned short int d)
{
  //wiringPiSPIDataRW ( 1 , ( unsigned char* ) &d , 2) ; //Send the data
} 

#endif






