
#ifndef  ILI9163_H
#define  ILI9163_H

#include "../GFX/GFX.h"

#include "TFTsettings.h"
#include "TFTregisters.h"

#include <stdio.h>
#include <stdlib.h>


class TFT : public GFX {
	#define WIDTH  128
	#define HEIGHT 128

public:
	void init ();
	
	TFT ( int cs, int rst,int dc );
	virtual inline void drawPixel     ( int16_t x, int16_t y, uint16_t color);
	virtual inline void drawFastVLine ( int16_t x, int16_t y, int16_t h, uint16_t color);
    virtual inline void drawFastHLine ( int16_t x, int16_t y, int16_t w, uint16_t color);

	void updateDisplay();
	void clearDisplay ();
	
	void setAddrWindow(unsigned short  x0, unsigned short  y0, unsigned short  x1, unsigned short  y1);
	void setRotation( unsigned char m);
	void colorSpace( unsigned char cspace);
	
	void initSpi();		
	inline void spiWrite     (unsigned char data);
	inline void writecommand (unsigned char c);
	inline void writedata    (unsigned char c);
	inline void writedata16  (unsigned short int d);
	inline void pushdata16   (unsigned short int d);
	
	uint16_t color565(uint8_t r, uint8_t g, uint8_t b) 
			{ uint16_t v = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
			  return ( v >> 8 ) | ( (v << 8 ) & 0xff00 ); 
			};
			
	
	
private:
	int csPin;    //  CS - Chip select, pin 3 on LCD.
	int rstPin;   //  RST - Reset, pin 4 on LCD.
	int dcPin;    //  DC - Data/Command, pin 5 on LCD.
	
	unsigned char _Mactrl_Data;
	unsigned char _colorspaceData;
	unsigned short displayMap[WIDTH][HEIGHT];

};

#endif
