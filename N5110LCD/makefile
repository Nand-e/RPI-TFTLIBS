

compile: test.o
	
build:	 5110LCD.o GFX.o test.o
	g++ -Wall -o test 5110LCD.o GFX.o test.o -lwiringPi 

test.o:	test.c ../GFX/GFX.h 
	g++ -Wall -c test.c -lwiringPi
	
5110LCD.o:  5110LCD.h
	g++ -Wall -c 5110LCD.cpp -lbcm2835 -lwiringPi 
	
GFX.o:	../GFX/GFX.cpp ../GFX/GFX.h
	g++ -Wall -c ../GFX/GFX.cpp
