/*
 * SGDSVM.cpp
 *
 *  Created on: Nov 24, 2017
 *      Author: adam
 */

#include "SGDSVM.h"

using namespace std;
SGD_SVM::SGD_SVM() {
	// TODO Auto-generated constructor stub

}

SGD_SVM::~SGD_SVM() {
	// TODO Auto-generated destructor stub
}



double SGD_SVM::dotP(map<double, double> row, map<double, double> weights) {
	double result = 0;
	map<double, double>::iterator iter = row.begin();
	while(iter != row.end()){
		double key = iter->first;
		double value = iter->second;
		double weight = weights[key];
		result += value*weight;
		iter++;
	}
//	cout << result << endl;
	return result;
}

void SGD_SVM::shuffle(){
	for(int line = labels.size() - 1; line > 0; line--){
		srand(time(NULL));

		int rnd = rand() % (line + 1);

		//swap map @ line with map @ rnd
		//need a temp map to swap
		map<double, double> swapmap = trainingMap.at(rnd);
		trainingMap.at(rnd) = trainingMap.at(line);
		trainingMap.at(line) = swapmap;

		//now swap the labels
		int swaplabel = labels.at(rnd);
		labels.at(rnd) = labels.at(line);
		labels.at(line) = swaplabel;
	}
}

void SGD_SVM::scale(double scalar, map<double, double> * vector){
	map<double, double>::iterator iter = vector->begin();
	while(iter != vector->end()){
		iter->second *= scalar;
		iter++;
	}
}


void SGD_SVM::run(double rate, double tradeoff){
	shuffle();
	for(int row = 0; row < labels.size(); row++){
		double result = dotP(trainingMap.at(row), weights);
		if(labels.at(row) * result <= 1){

			//first term
			scale(1 - rate, &weights);

			//second term
			scale(tradeoff, &trainingMap.at(row));
			scale(labels.at(row), &trainingMap.at(row));
			scale(rate, &trainingMap.at(row));

			//now see if there is a corresponding true feature value
			map<double, double>::iterator iter = trainingMap.at(row).begin();
		}else{
			scale(1-rate, &weights);
		}
	}
}

void SGD_SVM::stream(string filepath, bool isTest) {
	ifstream pipein(filepath.c_str());
	double right, wrong = 0;
	for (string line; getline(pipein, line);) {
		map<double, double> example;
		double label = ((line.at(0) == '0') ? -1 : 1);
		labels.push_back(label);
		line = line.substr(2); //2 was og 1
		std::string token;
		std::istringstream tokenStream(line);
		while (std::getline(tokenStream, token, ' ')) {
			float key = atof(token.c_str());
			example.insert(make_pair(key, 1));
		}
		if(!isTest){
			trainingMap.push_back(example);
		}else{
			double dot = dotP(example, weights);
			double temp = 0;
			if(dot <= 1){
				temp = -1;
			}else{
				temp = 1;
			}

			if(temp == label){
				right ++;
			}else{
				wrong ++;
			}
		}
	}
	if(isTest){
		cout << "Accuracy " << right / (right + wrong) << endl;
	}
}

void SGD_SVM::test(string filepath){
	stream(filepath, true);
}

void SGD_SVM::go() {


	stream(trainingFiles.at(0), false); //training map consists of the entire file now with positives.

	//for epoch 1....T
	run(rates.at(0), tradeoff.at(0));
	test("/data/speeches.test.liblinear");
}



