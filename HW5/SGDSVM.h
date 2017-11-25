/*
 * SGDSVM.h
 *
 *  Created on: Nov 24, 2017
 *      Author: adam
 */

#ifndef SGDSVM_H_
#define SGDSVM_H_
#include <stdlib.h>
#include <iostream>
#include <random>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>

struct point{
	double key = 0, value = 0;
};
using namespace std;
class SGD_SVM {
public:
//	map<double, double> trainingMap;
//	vector<point> trainingMap;
	vector<map<double, double>> trainingMap;
	vector<double> labels;
	map<double, double> weights; //index # to weight.
	vector<double> rates = {10, 1, 0.1, 0.01, 0.001, 0.0001};
	vector<double> tradeoff = {10, 1, 0.1, 0.01, 0.001, 0.0001};
	vector<string> trainingFiles = {
			"data/speeches.train.liblinear",
			"data/CVSplits/training00.data",
			"data/CVSplits/training01.data",
			"data/CVSplits/training02.data",
			"data/CVSplits/training03.data",
			"data/CVSplits/training04.data"
	};
	SGD_SVM();
	virtual ~SGD_SVM();
	void go();
	void shuffle();
	void stream(string filepath, bool isTest);
	void run(double rate, double tradeoff);
	void scale(double scalar, map<double, double> * vector);
	void updateLess(map<double, double> * vector, double rate, double tradeoff);
	double dotP(map<double, double> row, map<double, double> weights);
	void test(string filepath);

};

#endif /* SGDSVM_H_ */

//(a) Initial learning rate: γ 0 ∈ {10 1 , 10 0 , 10 −1 , 10 −2 , 10 −3 , 10 −4 }
//(b) The regularization/loss tradeoff parameter: C ∈ {10 1 , 10 0 , 10 −1 , 10 −2 , 10 −3 , 10 −4 }
