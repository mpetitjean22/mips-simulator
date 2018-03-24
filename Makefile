all: sim

sim: sim.cpp
	g++ -o sim sim.cpp UtilityFunctions.o -I ./
