

OUTFILE = test

compile: $(OUTFILE).o
	

build:	ILI9341.o GFX.o
	g++ -Wall -o $(OUTFILE) $(OUTFILE).o ILI9341.o GFX.o -lwiringPi  -lbcm2835

test.o:	test.c	../GFX/GFX.h ILI9341.h
	g++ -Wall -c test.c -lwiringPi


ILI9341.o: ../GFX/GFX.h ILI9341.h ILI9341.c
	g++ -Wall -c ILI9341.c -lwiringPi 

GFX.o:	../GFX/GFX.cpp ../GFX/GFX.h
	g++ -Wall -c ../GFX/GFX.cpp 
