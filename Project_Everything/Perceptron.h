/*
 * Perceptron.h
 *
 *  Created on: Dec 11, 2017
 *      Author: adam
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

#ifndef PERCEPTRON_H_
#define PERCEPTRON_H_
using namespace std;

class Perceptron {
public:
	Perceptron();
	virtual ~Perceptron();


	vector<double> weights;
	vector<int> predictions;
	void update(vector<double> example, double r);
	double dotP(vector<double> weights, vector<double> row);
	double perceptron(vector<double> example, bool isTest);
	void stream(string filepath, bool isTest);
	void populateWeights();
	void go();


};

#endif /* PERCEPTRON_H_ */
