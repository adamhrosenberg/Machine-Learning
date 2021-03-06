#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <sstream>

#include "Bagged.h"
#include "ID3.h"
#include "KNN.h"
#include "Perceptron.h"
#include "KMeans.h"
#include "LogisticRegression.h"
#include "SGDSVM.h"


int main(){

	Bagged forests;
	forests.bagged(false);

	ID3 id;
	id.go();

	Perceptron per;
	per.go();


	KMeans km;
	km.go();

	KNN knn;
	knn.go();

	LogisticRegression log;
	log.go();

	SGDSVM svm;
	svm.go();


}
