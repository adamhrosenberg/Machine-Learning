/*
 * ID3.h
 *
 *  Created on: Dec 1, 2017
 *      Author: adam
 */

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
//#include "BaggedSVM.h"
//#include "BaggedLogistic.h"
//#include "Files.h"

#ifndef ID3_H_
#define ID3_H_

using namespace std;

typedef struct TreeNode {
public:
	string attribute;
	int attributeNum;
	vector<TreeNode> branches;
	bool leaf;
	bool label_value;
} TreeNode;


class ID3 {
public:
	ID3();
	virtual ~ID3();
	void go();
	void stream(string filepath, bool isTest);
	void test(string filepath);
	void run(vector<map<double, double>> * S, vector<double> * l, set<int> * attributes);


	void bagged(bool isSVM);

	TreeNode recurse(vector<map<double, double>> * S, set<int> * attributes, vector<double> * labels, int currentDepth);

	int bestAttributeThatClassifiesS(vector<map<double, double>> * S, set<int> * attributes, vector<double> * l, double totalEntropy);

	double calculateGain(vector<map<double, double>> * S, vector<double> * l, double totalEntropy, int featureNumber);

	double calculateTotalEntropy(vector<double> * l);

	TreeNode _root;

	vector<TreeNode> forest;
//	vector<Type> *vect = new vector<Type>;

	double calculateEntropy(vector<map<double,double>> * S, vector<double> * l, int featureNumber);
private:
	vector<string> trainingFiles = {
			"data-splits/data.train"};

//	vector<string>trainingFiles;
	vector<string> testingFiles = { "data-splits/data.test"};
//	vector<string> testingFiles = { "data-splits/data.eval.anon"};

	vector<map<double, double>> trainingMap;
	set<double> zeroData;
	vector<double> labels;
	bool allLabelsTrue = true;
	int numpre =0;

	void getAverages(string filepath);

	vector<map<double, double>> mapSVM;
	vector<double> labelsSVM;
//	vector<TreeNode> forest;
	map<double, double> weightsSVM;

	vector<map<double, double>> mapLog;
	vector<double> labelsLog;
	map<double, double> weightsLog;

	set<int> featuresMentioned;
	int possibleValuesForA = 2;
	int maxDepth = 4;
	bool seeIfAllTrue(vector<double> * l);
	vector<double> discretizedFeatureValues;
	vector<int> predictions;
	vector<int> numberOfTimesFeatureMentioned;


};


#endif /* ID3_H_ */
