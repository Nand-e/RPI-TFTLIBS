#ifndef ILI9341_H
#define ILI9341_H


#include "../GFX/GFX.h"

class TFT : public GFX {
public:
		TFT( int dc, int reset, int cs );
		void init  ();
		
		inline void drawPixel(int16_t x, int16_t y, uint16_t color);
		void  setAddrWindow( short int x0, short int y0, short int x1, short int y1);
		void  drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
		void  drawFastHLine(int16_t x, int16_t y,int16_t w, uint16_t color);
		
		void initSpi();
		inline void spiWrite ( unsigned char c);
		inline void writeCommand ( unsigned char c );
		inline void writeData    ( unsigned char c );
		inline void writeData16 ( unsigned short int d);
		inline void pushData16( unsigned short int d);
	

		void  pushColor ( unsigned short int color );
		void  sendLine ( unsigned short int y);		
		void  updateDisplay();
	
		unsigned short displayMap[320][240];

private:
		int dcPin, rstPin, csPin;
		
	
};
 
#endif
