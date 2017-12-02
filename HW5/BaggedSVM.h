
/*
 * BaggedSVM.h
 *
 *  Created on: Dec 2, 2017
 *      Author: adam
 */

#ifndef BAGGEDSVM_H_
#define BAGGEDSVM_H_
#include <stdlib.h>
#include <iostream>
#include <random>
#include <fstream>
#include <string>
#include <vector>
#include <utility>      // std::pair, std::make_pair
#include <algorithm>
#include <sstream>
#include <map>
using namespace std;


class BaggedSVM {
public:
	vector<map<double, double>> trainingMap;
	vector<double> labels;
	vector<double> accuracies;



	double averagePercentage = 0;
	map<pair<double, double>, double> accuracyPoints;
	double percentageCross = 0;
	double averageOfCross = 0;
	int numberOfCross = 0;
	map<double, double> weights; //index # to weight.
	vector<double> rates = {10, 1, 0.1, 0.01, 0.001, 0.0001}; //4
	vector<double> tradeoff = {10, 1, 0.1, 0.01, 0.001, 0.0001}; //2
	vector<string> trainingFiles = {
			"data/CVSplits/training00.data",
			"data/CVSplits/training01.data",
			"data/CVSplits/training02.data",
			"data/CVSplits/training03.data",
			"data/CVSplits/training04.data"
	};
	BaggedSVM();
	virtual ~BaggedSVM();
	void go();
	string pickTraining(int against);
	double t = 0;
	int numpre =0;
	void shuffle();
	void stream(string filepath, bool isTest);

	void run(double rate, double tradeoff);

	void scale(double scalar, map<double, double> * vector);
	void updateLess(map<double, double> * vector, double rate, double tradeoff);
	double dotP(map<double, double> row, map<double, double> weights);
	void test(string filepath);
	void crossValidate(double rate, double tradeoff);
	int count = 0;
	double gamma_t = 0;

};

#endif /* SGDSVM_H_ */

//(a) Initial learning rate: γ 0 ∈ {10 1 , 10 0 , 10 −1 , 10 −2 , 10 −3 , 10 −4 }
//(b) The regularization/loss tradeoff parameter: C ∈ {10 1 , 10 0 , 10 −1 , 10 −2 , 10 −3 , 10 −4 }
