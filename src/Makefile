FLAGS = -g -lpthread -fopenmp -O3 -I ${HOME}/fastflow -std=c++20

main: main.o Jacobi.o
	g++ -o main main.o Jacobi.o $(FLAGS)

main.o: main.cpp Jacobi.hpp utils.hpp
	g++ -c main.cpp $(FLAGS)

Jacobi.o: Jacobi.cpp Jacobi.hpp utils.hpp
	g++ -c Jacobi.cpp $(FLAGS)