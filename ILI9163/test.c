

#include <stdio.h>
#include <stdlib.h>

#include "ILI9163.h"

#include <wiringPi.h>


class Ball {
public:	

    Ball() {
		speedx = speedy = 2;
	};
	Ball ( int x1, int y1) {
		x = x1;
		y = y1;
		speedx = 2;
		speedy = 2;
	}
	
	void update () {
		if ( x >= 124) speedx = -2;
	    if ( x <= 4 ) speedx =  +2;
	    x+= speedx;
		if ( y > 124) speedy =  -1.8 * abs( speedy );
		if ( y <  5  ) speedy = + 1;
		y+= speedy;
		
		speedy += 0.1f * r;
		if ( speedy > 3 ) speedy =3 ; 
	}
	
	int speedx;
	float speedy;
	int x;
	int y;
	uint16_t color; 
	int r;
};
	

int main(int argc, char **argv)
{
//    TFT tft ( 21,22,23);		// with wiringpilibrary
	TFT  tft (  5, 6 ,13 );		// with bmc library zou can choose this lib in ILI9163.c file
    tft.init();

	Ball balls[50];

    
    
   
	for ( int i =0; i < 50; i ++ ) {
		balls[i].x = rand () % 100 +10; 
		balls[i].y = rand () % 100 +10; 
		balls[i].color = rand () % 65535;
		balls[i].r = 1 + rand () % 5;
  	}
  
    char s1[] = { "Rpi" };
	tft.setTextColor ( tft.color565 (0,0,255) , tft.color565 (255,0,0) );

	while (1) {	
		tft.fillRect ( 0, 0,128, 10, tft.color565 (255, 0,  0 ));
		tft.fillRect ( 0,10,128, 10, tft.color565 ( 0 , 255,0 ));
		tft.fillRect ( 0,20,128, 10, tft.color565 ( 0 , 0,255 ));

		tft.drawString ( s1, balls[0].x, balls[0].y, 2 );
		tft.drawRect ( 0,0,128,127 , tft.color565 ( 0,0,255 ) );
	
		for ( int i =0; i < 50; i ++ ) {
			balls[i].update();
			tft.fillCircle( balls[i].x,  balls[i].y, balls[i].r ,balls[i].color); 
		}	  
		tft.updateDisplay ();
		delay (10);
	}
	return 0;
}

