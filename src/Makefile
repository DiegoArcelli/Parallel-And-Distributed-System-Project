FLAGS = -g -lpthread -fopenmp -O3 -I ${HOME}/fastflow -std=c++20

main: main.o Jacobi.o timer.o
	g++ -o main main.o Jacobi.o timer.o $(FLAGS)

main.o: main.cpp Jacobi.hpp utils.hpp timer.hpp
	g++ -c main.cpp $(FLAGS)

Jacobi.o: Jacobi.cpp Jacobi.hpp utils.hpp
	g++ -c Jacobi.cpp $(FLAGS)

timer.o: timer.cpp timer.hpp
	g++ -c timer.cpp