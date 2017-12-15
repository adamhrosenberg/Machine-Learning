/*
 * KMeans.h
 *
 *  Created on: Dec 12, 2017
 *      Author: adam
 */

#ifndef KMEANS_H_
#define KMEANS_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <random>
#include <limits>
#include <sstream>

using namespace std;

struct mPoint {
	vector<double> coordinates; //size 17.
	double distance; //distance from point to the mean centroid.
	int cluster_id;
	bool isCluster;
	int numOfPointsInCluster;
	int runningTotal;
	bool label;
};

class KMeans {
public:
	KMeans();
	virtual ~KMeans();
	static bool comparison(mPoint a, mPoint b);
	double computeDistance(vector<double> p1, vector<double> p2);
	double computeManhattanDistance(vector<double> p1, vector<double> p2);
	void createTrainingData(string filepath, bool isTest);
	void go();
	void KM();
	void populateCentroids();
	void calculateClusters();
	vector<double> add(vector<double> p1, vector<double> p2);
	void divide(vector<double> * p1, int scale);
	void test();

	vector<string> userIDs;
	int userCount = 0;

	double accuracy;

	vector<int> predictions;
	vector<double> labels;
	//vector<point> distances;
//	vector<int>
	vector<mPoint> trainingData; //every point in the training file
	vector<mPoint> centroids;


	int k;
};

#endif /* KMEANS_H_ */
