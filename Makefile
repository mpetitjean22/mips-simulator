CC = g++
FLAGS = -Wall

all: sim

clean:
	rm sim

sim: sim.cpp decode.o register.o instr.o
	$(CC) $(FLAGS) -o sim sim.cpp UtilityFunctions.o -I ./
