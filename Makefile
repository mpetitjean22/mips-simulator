CC = g++
FLAGS = -Wall

all: sim

clean:
	rm sim

sim: sim.cpp
	$(CC) $(FLAGS) -o sim sim.cpp UtilityFunctions.o -I ./
