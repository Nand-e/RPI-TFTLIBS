


compile: test.o
	

build:	 ILI9163.o GFX.o
	g++ -Wall -o test test.o ILI9163.o GFX.o -lwiringPi -lbcm2835

test.o:	test.c	../GFX/GFX.h ILI9163.h TFTsettings.h
	g++ -Wall -c test.c -lwiringPi
	
ILI9163.o:  TFTsettings.h ../GFX/GFX.h ILI9163.h ILI9163.c
	g++ -Wall -c -lwiringPi ILI9163.c -lbcm2835

GFX.o:	../GFX/GFX.cpp ../GFX/GFX.h
	g++ -Wall -c ../GFX/GFX.cpp

