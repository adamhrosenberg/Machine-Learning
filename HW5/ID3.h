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
	void run(int depth);
	double calculateEntropy(vector<map<double,double>> S, vector<double> l, int featureNumber);
	TreeNode recurse(vector<map<double, double>> S, set<int> attributes, vector<double> labels);
	int bestAttributeThatClassifiesS(vector<map<double, double>> S, set<int> attributes, vector<double> l, double totalEntropy);
	double calculateGain(vector<map<double, double>> S, vector<double> l, double totalEntropy, int featureNumber);
	double calculateTotalEntropy(vector<double> l);
private:
	vector<string> trainingFiles = {
			"data/CVSplits/training00.data", "data/CVSplits/training01.data",
			"data/CVSplits/training02.data", "data/CVSplits/training03.data",
			"data/CVSplits/training04.data" };
	vector<map<double, double>> trainingMap;
	vector<double> labels;
	bool allLabelsTrue = true;
	set<int> featuresMentioned;
	int possibleValuesForA = 2;
	bool seeIfAllTrue(vector<double> l);


};


#endif /* ID3_H_ */
