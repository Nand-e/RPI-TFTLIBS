
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "ILI9341.h"

using namespace std;


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
		if ( x >= 300 ) speedx = -4;
	    if ( x <= 20 ) speedx = +4;
	    x+= speedx;
		if ( y > 220 ) speedy =  -1.6f * abs( speedy );
		if ( y < 20  ) speedy = +2;
		y+= speedy;

		speedy += 0.1f * r;
		if ( speedy > 5 ) speedy =5; 
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
 // TFT tft ( 21 ,22 ,23  );    // with wiringpi library 
  TFT tft ( 5, 6, 13 );			// with bcm library zou can set in ILI9341.c file
  tft.init();
  tft.setAddrWindow( 0,0, 320, 240 );
  
	
    Ball balls[100];
  
   for ( int i =0; i < 100; i ++ ) {
	 balls[i].x = rand () % 280 +20; 
 	 balls[i].y = rand () % 200 +20; 
     balls[i].color = rand () % 65500;
	 balls[i].r = 2 + rand () % 8;  
   }
    
   do {
		for ( int i =0; i < 100; i ++ ) {
			balls[i].update();
			tft.fillCircle ( balls[i].x,  240 - balls[i].y, balls[i].r , balls[i].color); 
		}
	   tft.updateDisplay ();
  } while(1);
  return 0;
}


