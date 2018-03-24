CC = g++
FLAGS = -Wall

all: sim

clean:
	rm sim decode.o register.o

decode.o: decode.h decode.cpp
	$(CC) $(FLAGS) -c decode.cpp

register.o: register.h register.cpp
	$(CC) $(FLAGS) -c register.cpp

sim: sim.cpp decode.o register.o
	$(CC) $(FLAGS) -o sim sim.cpp UtilityFunctions.o decode.o register.o -I ./
