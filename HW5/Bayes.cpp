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
		if(label == 1){
			numLabelTrue ++;
		}
		line = line.substr(2); //2 was og 1
		std::string token;
		std::istringstream tokenStream(line);
		float key;
		featuresMentioned.push_back(key);
		while (std::getline(tokenStream, token, ' ')) {
			key = atof(token.c_str());
			example.insert(make_pair(key, 1));
		}
		if (!isTest) {
			labels.push_back(label);
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
void Bayes:: computeProbabilityTable(){
	for (int featureNum = 0; featureNum < featuresMentioned.size(); featureNum++) {
		double numFeatureTrue = 0; //where x1 is and the label is true.
		double trueCount = 0; //number of times x is true and y is true
		double falseCount = 0; //number of times x is false and y is true
		for (int trainingMapRow = 0; trainingMapRow < trainingMap.size(); trainingMapRow++) {
			map<double, double>::iterator iter = trainingMap.at(trainingMapRow).begin();
			while (iter != trainingMap.at(trainingMapRow).end()) {
				//for each item in the feature map, create
				//iter->first is the key avlue in trainingMap. which is the feature #.
				if (iter->first == featuresMentioned.at(featureNum)) {
					if(labels.at(trainingMapRow) == 1){
						numFeatureTrue ++;
						//now do for 0 and 1
						if(iter->second == -1){
							falseCount ++;
						}else if(iter->second == 1){
							trueCount ++;
						}
					}

				}
				iter++;
			}
		}

		//now after every row.
		ProbPair entry;
		entry.pTrue = trueCount / numFeatureTrue;
		entry.pFalse = falseCount / numFeatureTrue;
		probabilityTable[featureNum] = entry;
	}
}

void Bayes:: go(){
	stream(trainingFiles.at(0), false);
	cout << "Prior: " << prior << "\n labels marked true: " << numLabelTrue << endl;
	computeProbabilityTable();
}

