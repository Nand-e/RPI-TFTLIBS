
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include <stdio.h>
#include <stdlib.h>

#include "5110LCD.h"



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
		if ( x >= 80 ) speedx = -2;
	    if ( x <= 4 ) speedx =  +2;
	    x+= speedx;
		if ( y >  45 ) speedy =  -1.1f * abs( speedy );
		if ( y <  5  ) speedy = + 1;
		y+= speedy;
		
		speedy += 0.2f;
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
	NOKIALCD lcd ( 21,22,23 );
	lcd.init();
	lcd.setContrast( 35);

	Ball balls[15];
  
	for ( int i =0; i < 15; i ++ ) {
		balls[i].x = rand () % 70 +10; 
		balls[i].y = rand () % 40 +10; 
		balls[i].color = 100;
		balls[i].r = 1 + rand () % 3;
  
	}
    char s1[] = { "Rpi" };
	int s = 0;
	
	lcd.setTextColor ( 0, 1 );
	while (1) {	
		s++;
		s%= 128;
		
		
		lcd.drawString ( s1, balls[0].x, balls[0].y, 2 );
		for ( int i =0; i < 15; i ++ ) {
			balls[i].update();
			lcd.fillCircle ( balls[i].x,  balls[i].y, balls[i].r , balls[i].color); 
		}	  

		lcd.updateDisplay();
		delay ( 80 );
	}
	return 0;
}

