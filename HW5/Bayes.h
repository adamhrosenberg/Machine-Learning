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
#include <math.h>
#include <algorithm>
#include <sstream>
#include <map>
#include <set>
#include "Files.h"
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
//	int numTrainExamples = 2818;
//	int numTestExamples = 940;
	double prior = 0;
	double numLabelTrue = 0;
	void go();
	void test(string filepath);
	set<int> featuresMentioned;
	map<pair<double, double>, double> accuracyPoints;
	double percentageCross = 0;
	double averageOfCross = 0;
	vector<double> accuracies;
	double averagePercentage = 0;
	int numberOfCross = 0;
	vector<map<double, double>> trainingMap;
//	map<double, double> featureMap; //index # to weight.
	vector<double> labels;
	map<int, ProbPair> probabilityTable; //each feature index has a corresponding pair, which consists of the Probability of it being true / false given the dataset..
	string pickTraining(int against);

	vector<double> smoothness = { 2, 1.5, 1, .5 };
//	vector<double> smoothness = {2};
//	vector<string> trainingFiles = {
//			"data/speeches.train.liblinear",
//			"data/CVSplits/training00.data",
//			"data/CVSplits/training01.data",
//			"data/CVSplits/training02.data",
//			"data/CVSplits/training03.data",
//			"data/CVSplits/training04.data"
//	};
	vector<string> trainingFiles;
	vector<string> testingFiles;
	void computeProbabilityTable(double smoothing);
	void stream(string filepath, bool isTest);
	void crossValidate(double smoothness);
};

#endif /* BAYES_H_ */
