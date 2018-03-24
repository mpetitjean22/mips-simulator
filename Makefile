all: sim

decode.o: decode.h decode.c
	gcc -c decode.c

sim: sim.cpp decode.o
	g++ -o sim sim.cpp UtilityFunctions.o decode.o -I ./
