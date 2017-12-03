/*
 * LogisticRegression.h
 *
 *  Created on: Nov 28, 2017
 *      Author: adam
 */

#ifndef LOGISTICREGRESSION_H_
#define LOGISTICREGRESSION_H_

#include <stdlib.h>
#include <iostream>
#include <random>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>
#include <sstream>
#include <map>
#include <set>
#include "Files.h"

using namespace std;

class LogisticRegression {
public:
	LogisticRegression();
	virtual ~LogisticRegression();
	void go();
	double dotP(map<double, double> row, map<double, double> weights);
	void scale(double scalar, map<double, double> * vector);
	void shuffle();
	void stream(string filepath, bool isTest);
	void updateWeights(double rate, double tradeoff);
	map<double, double> mapAdd(map<double, double> * weights, map<double, double> * example);
	void test(string filepath);
	void crossValidate(double rate, double tradeoff);
	string pickTraining(int against);
private:
	//	map<double, double> trainingMap;
	//	vector<point> trainingMap;
	vector<map<double, double>> trainingMap;
	vector<double> labels;
	double gamma_t;
	map<pair<double, double>, double> accuracyPoints;
	double percentageCross = 0;
	double averageOfCross = 0;
	vector<double> accuracies;
	double averagePercentage = 0;
	int numberOfCross = 0;
	map<double, double> weights; //index # to weight.
	void run(double rate, double tradeoff);
	double t = 0;
	vector<double> rates = {1, 0.1, 0.01, 0.001, 0.0001, 0.00001 };
	vector<double> tradeoff = { 0.1, 1, 10, 100, 1000, 10000 };
//	vector<double> rates = {1};
//	vector<double> tradeoff = {1};
//	vector<string> trainingFiles = {
//			"data/CVSplits/training00.data", "data/CVSplits/training01.data",
//			"data/CVSplits/training02.data", "data/CVSplits/training03.data",
//			"data/CVSplits/training04.data" };
	vector<string> trainingFiles;
	vector<string> testingFiles;
};

#endif /* LOGISTICREGRESSION_H_ */
