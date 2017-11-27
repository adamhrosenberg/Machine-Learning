/*
 * Bayes.h
 *
 *  Created on: Nov 26, 2017
 *      Author: adam
 */

#ifndef BAYES_H_
#define BAYES_H_
#include <stdlib.h>
#include <iostream>
#include <random>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
#include <set>
//#include <point>
using namespace std;

typedef struct ProbPair{
	double pTrue;
	double pFalse;
}ProbPair;

class Bayes {
public:
	Bayes();
	virtual ~Bayes();
//	int numFeatures = 67692;
	int numTrainExamples = 2818;
	int numTestExamples = 940;
	double prior = 0;
	double numLabelTrue = 0;
	void go();
	void test(string filepath);
	set<int> featuresMentioned;
	vector<map<double, double>> trainingMap;
//	map<double, double> featureMap; //index # to weight.
	vector<double> labels;
	map<int, ProbPair> probabilityTable; //each feature index has a corresponding pair, which consists of the Probability of it being true / false given the dataset..
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
	void computeProbabilityTable();
	void stream(string filepath, bool isTest);
};

#endif /* BAYES_H_ */
