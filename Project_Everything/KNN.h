/*
 * KNN.h
 *
 *  Created on: Dec 11, 2017
 *      Author: adam
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <sstream>

using namespace std;

struct point {
	vector<double> coordinates; //size 17.
	double distance; //distance from ponit to the test point.
};
class KNN {

public:
	KNN();
	virtual ~KNN();
	void go();
	void NN(string filepath);
	void createTrainingData(string filepath, bool isTest);
	double computeDistance(vector<double> p1, vector<double> p2);
	static bool comparison(point a, point b);

	vector<string> userIDs;
	int userCount = 0;
	vector<int> predictions;
	//vector<point> distances;
	vector<point> trainingData; //every point in the training file
};

