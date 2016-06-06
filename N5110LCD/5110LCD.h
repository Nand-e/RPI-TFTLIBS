

#include "../GFX/GFX.h"


/* 84x48 LCD Defines: */
#define LCD_WIDTH   84 // Note: x-coordinates go wide
#define LCD_HEIGHT  48 // Note: y-coordinates go high
#define WHITE       0  // For drawing pixels. A 0 draws white.
#define BLACK       1  // A 1 draws black.

class NOKIALCD : public GFX {

public:
	void init ();
	
	NOKIALCD ( int dc,int ce, int rst ); 
	void drawPixel( int16_t x, int16_t y, uint16_t color);
	
	void updateDisplay();
	void clearDisplay( bool bw);
	void setContrast( unsigned char contrast);
	void LCDWrite( unsigned char data_or_command, unsigned char data);

	int scePin; //  SCE - Chip select, pin 3 on LCD.
	int rstPin; //  RST - Reset, pin 4 on LCD.
	int dcPin;  //  DC  - Data/Command, pin 5 on LCD.
	
	unsigned char displayMap[LCD_WIDTH * LCD_HEIGHT / 8];

};
