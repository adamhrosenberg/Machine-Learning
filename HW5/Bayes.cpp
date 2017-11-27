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
			//compute the prior probablitiy.


//			labels.push_back(label);
			trainingMap.push_back(example);

		} else {
			double guess;
			//make prediction
			//we have example. which is a map.
			double sumT = 1;
			double sumF = 1;
			map<double, double>::iterator examIter = example.begin();
			for (; examIter != example.end(); examIter++) {
				double guess = 0;
				sumT *= probabilityTable[examIter->first].pTrue * prior;
				sumF *= probabilityTable[examIter->first].pFalse * prior;
			}
			if (sumT >= sumF) {
				guess = 1;
			} else {
				guess = -1;
			}

			cout << guess << " T: " << sumT << " F " << sumF <<  " for the true prob =  "
					<< probabilityTable[examIter->first].pTrue << " , false = "
					<< probabilityTable[examIter->first].pFalse
					<< endl;


			if (guess == label)
				right++;
			else
				wrong++;
		}
	}

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

		double trueCount = 0; //number of times x is true and y is true
		double falseCount = 0; //number of times x is false and y is true

		for (int trainingMapRow = 0; trainingMapRow < trainingMap.size();
				trainingMapRow++) {
			//for each row of the trainingmap, iterate through the row until the features line up. then check the label.

			map<double, double>::iterator trainIter;
			double featureNumber = *featIter;
			trainIter = trainingMap.at(trainingMapRow).find(featureNumber);
			if(trainIter != trainingMap.at(trainingMapRow).end()){
				//found it.
				if(labels.at(trainingMapRow) == 1){
					trueCount ++;
				}else{
					falseCount ++;
				}
			}

//			new row.
//			cout << "new row : #  " << trainingMapRow << endl;
		}

		//now after every row.
		ProbPair entry;
		entry.pTrue = (trueCount + 1) / (numLabelTrue + 2);
		entry.pFalse = (falseCount + 1) / ((featuresMentioned.size() - numLabelTrue) + 2);

		probabilityTable[*featIter] = entry;

		cout << "Probabilities for feature: " << *featIter << " "  << entry.pFalse << " " << entry.pTrue << endl;
//
//		cout << " true count, false count " << trueCount << " " << falseCount << endl;
		featIter++;
	}
}

void Bayes::test(string filepath) {
	stream(filepath, true);
}
void Bayes::go() {
	stream(trainingFiles.at(0), false);
	cout << "size of training map: " << trainingMap.size() << endl;
	cout << "size of labels " << labels.size() << endl;
	cout << "featuredmentioend size " << featuresMentioned.size() << endl;

	prior = numLabelTrue / featuresMentioned.size(); //CHECK THIS BEFORE CHANING FILES

	cout << "Prior: " << prior << endl;

	computeProbabilityTable();

	test("data/speeches.test.liblinear");
//	cout << "size of training map: " << trainingMap.size() << endl;
	cout << "done" << endl;
}

