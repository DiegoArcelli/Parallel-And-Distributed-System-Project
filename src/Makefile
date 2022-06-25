.PHONY: clean all

FFDIR = ${HOME}/fastflow
FLAGS = -g -lpthread -fopenmp -I ${FFDIR} -std=c++20 -O3
COMP = g++

all: main clean

main: main.o Jacobi.o timer.o
	$(COMP) -o main main.o Jacobi.o timer.o $(FLAGS)

main.o: main.cpp Jacobi.hpp utils.hpp timer.hpp
	$(COMP) -c main.cpp $(FLAGS)

Jacobi.o: Jacobi.cpp Jacobi.hpp utils.hpp
	$(COMP) -c Jacobi.cpp $(FLAGS)

timer.o: timer.cpp timer.hpp
	$(COMP) -c timer.cpp

clean:
	@rm *.o