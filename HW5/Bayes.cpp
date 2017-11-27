/*
 * Bayes.cpp
 *
 *  Created on: Nov 26, 2017
 *      Author: adam
 */

#include "Bayes.h"

Bayes::Bayes() {
	// TODO Auto-generated constructor stub

}

Bayes::~Bayes() {
	// TODO Auto-generated destructor stub
}

void Bayes::stream(string filepath, bool isTest) {
	ifstream pipein(filepath.c_str());
	double right = 0, wrong = 0;
	for (string line; getline(pipein, line);) {
		map<double, double> example;
		double label = ((line.at(0) == '-') ? -1 : 1);
		labels.push_back(label);
		if (label == 1) {
			numLabelTrue++;
		}
		line = line.substr(2); //2 was og 1
		std::string token;
		std::istringstream tokenStream(line);
		float key;

		while (std::getline(tokenStream, token, ' ')) {
			key = atof(token.c_str());
			example.insert(make_pair(key, 1));
			featuresMentioned.insert(key);
		}
		if (!isTest) {
//			labels.push_back(label);
			trainingMap.push_back(example);

		} else {
			//make prediction
		}
	}

	//compute the prior probablitiy.
	prior = numLabelTrue / numTrainExamples; //CHECK THIS BEFORE CHANING FILES

	if (isTest) {
		cout << "Accuracy " << right / (right + wrong) << endl;
		cout << "Right: " << right << " wrong: " << wrong << endl;
	}
}
void Bayes::computeProbabilityTable() {
	double smoothing = 1;
	double count = 0;
	cout << "Starting prob table" << endl;
	set<int>::iterator featIter = featuresMentioned.begin();

	//iterate through the featurementioned set.
	while (featIter != featuresMentioned.end()) {
//		cout << "new feature num " << featuresMentioned.at(featureNum) << endl;

		//num:
		//we need to keep track of when x is true and the label is true.
		//we need to keep track of when x is true and the label is false.

		//denom:
		//we need to keep track of the number of when x isnt in the row and the label is true
		//we need to keep track of the number of when x isnt in the row and the label is false


		double numLabelTrue = 0; //where x1 is and the label is true.
		double numLabelFalse = 0;

		double trueCount = 0; //number of times x is true and y is true
		double falseCount = 0; //number of times x is false and y is true

		for (int trainingMapRow = 0; trainingMapRow < 1000 /*trainingMap.size()*/; trainingMapRow++) {
			//for each row of the trainingmap, iterate through the row until the features line up. then check the label.
			map<double, double>::iterator iter =
					trainingMap.at(trainingMapRow).begin();

			while (iter != trainingMap.at(trainingMapRow).end()) {

				if(*featIter == iter->first){
					//feature is 1.
//					cout << trainingMap.at(trainingMapRow).at(*featIter) << endl;

					if(labels.at(trainingMapRow) == 1){
						numLabelTrue++;
						trueCount++;
//						cout << "positive label for iter->first / *featIter = " << iter->first << endl;
//						trueCount++;
					}else{
						numLabelFalse++;
						falseCount++;
//						cout << "neagtive label for iter->first / *featIter = " << iter->first << endl;
					}
				}
				iter++;
				if(iter == trainingMap.at(trainingMapRow).end()){
					//the feature was not in the row
					//if the labels is 1 then increment true count.
					if(labels.at(trainingMapRow) == 1)
						numLabelTrue++;
					else
						numLabelFalse++;
				}
			}
			//new row.
//			cout << "new row : #  " << trainingMapRow << endl;
		}

		//now after every row.
		ProbPair entry;
		entry.pFalse = falseCount / numLabelFalse;
		entry.pTrue = trueCount / numLabelTrue;

		probabilityTable[*featIter] = entry;

		cout << "Probabilities: " << entry.pFalse << " " << entry.pTrue << endl;

//		cout << " label true # / label false # " << numLabelTrue << " " << numLabelFalse << endl;
//		cout << " true count, false count " << trueCount << " " << falseCount << endl;
		featIter++;
	}
}

void Bayes::go() {
	stream(trainingFiles.at(0), false);
	cout << "size of training map: " << trainingMap.size() << endl;
	cout << "size of labels " << labels.size() << endl;
	cout << "Prior: " << prior << "\n labels marked true: " << numLabelTrue
			<< endl;

//		set<int>::iterator iter = featuresMentioned.begin();
//		while (iter != featuresMentioned.end()) {
//			cout << *iter << endl;
//			iter++;
//		}


	computeProbabilityTable();
	cout << "done" << endl;
}

