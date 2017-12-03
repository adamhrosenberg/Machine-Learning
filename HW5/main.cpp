#include <stdlib.h>
#include <iostream>

#include "SGDSVM.h"
#include "Bayes.h"
#include "LogisticRegression.h"
#include "ID3.h"

using namespace std;



int main(){

//	cout << "Running SGD SVM " << endl;
//	SGD_SVM svm;
//	svm.go();


//	cout << "\n\nRunning Bayes " << endl;
//	Bayes bayes;
//	bayes.go();

//	cout << "\n\nRunning logisitic regression " << endl;
//	LogisticRegression logic;
//	logic.go();

	cout << "\n\nRunning bagged forest " << endl;
	ID3 id;


//	for(int i = 0; i < 100; i++){
	//this runs bagged and svm over bagged
	id.bagged(false);

//	ID3 idSVM;
//	idSVM.bagged(true);
//	id.go();
//	}
}
