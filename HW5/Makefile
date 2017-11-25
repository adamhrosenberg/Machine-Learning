CC = g++
OBJ = SGDSVM.o main.o
DEPS = SGDSVM.h
FLAGS = -g -std=c++11
all: SGDSVM

SGDSVM:$(OBJ)
	$(CC) $(FLAGS) -o SGDSVM $(OBJ)
main.o:main.cpp
	$(CC) $(FLAGS) -c main.cpp
SGDSVM.o: SGDSVM.cpp
	$(CC) $(FLAGS) -c SGDSVM.cpp