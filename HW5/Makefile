CC = g++
OBJ = SGDSVM.o main.o Bayes.o ID3.o LogisticRegression.o BaggedSVM.o
DEPS = SGDSVM.h Bayes.h ID3.h LogisticRegression.h BaggedSVM.h
FLAGS = -g -std=c++11
all: hw5

hw5: $(OBJ)
	$(CC) $(FLAGS) -o hw5 $(OBJ)
SGDSVM:SGDSVM.cpp
	$(CC) $(FLAGS) -c SGDSVM.cpp
main.o:main.cpp
	$(CC) $(FLAGS) -c main.cpp
SGDSVM.o: SGDSVM.cpp
	$(CC) $(FLAGS) -c SGDSVM.cpp
Bayes.o:Bayes.cpp
	$(CC) $(FLAGS) -c Bayes.cpp
LogisticRegression.o: LogisticRegression.cpp
	$(CC) $(FLAGS) -c LogisticRegression.cpp
ID3.o:ID3.cpp
	$(CC) $(FLAGS) -c ID3.cpp
BaggedSVM.o:BaggedSVM.cpp
	$(CC) $(FLAGS) -c BaggedSVM.cpp

clean:
	rm -f svm $(OBJ)

