/*
 * Bayes.cpp
 *
 *  Created on: Nov 26, 2017
 *      Author: adam
 */

#include "Bayes.h"

Bayes::Bayes() {
	Files f;
	trainingFiles = f.trainingFiles;
	testingFiles = f.testingFiles;
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
			double sumT = 0;
			double sumF = 0;
			map<double, double>::iterator examIter = example.begin();
			for (; examIter != example.end(); examIter++) {
				double guess = 0;
				sumT += log2(probabilityTable[examIter->first].pTrue * prior);
				sumF += log2(probabilityTable[examIter->first].pFalse* prior);
			}
			if (sumT >= sumF) {
				guess = 1;
			} else {
				guess = -1;
			}

//			cout << guess << " T: " << sumT << " F " << sumF <<  " for the true prob =  "
//					<< probabilityTable[examIter->first].pTrue << " , false = "
//					<< probabilityTable[examIter->first].pFalse
//					<< endl;

			if (guess == label)
				right++;
			else
				wrong++;
		}
	}

	if (isTest) {
		cout << "\tAccuracy " << right / (right + wrong) << endl;
		percentageCross = right / (right + wrong);
	}
}
void Bayes::computeProbabilityTable(double smoothing) {
//	double smoothing = 1;
	double count = 0;
//	cout << "Starting prob table" << endl;
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
			if (trainIter != trainingMap.at(trainingMapRow).end()) {
				//found it.
				if (labels.at(trainingMapRow) == 1) {
					trueCount++;
				} else {
					falseCount++;
				}
			}else{
				if (labels.at(trainingMapRow) == 1) {
					trueCount++;
				} else {
					falseCount++;
				}
			}

		}

		//now after every row.
		ProbPair entry;
		entry.pTrue = (trueCount + smoothing) / (numLabelTrue + smoothing);
		entry.pFalse = (falseCount + smoothing)
				/ ((featuresMentioned.size() - numLabelTrue) + 2 * smoothing);

		probabilityTable[*featIter] = entry;

//		cout << "Probabilities for feature: " << *featIter << " "  << entry.pFalse << " " << entry.pTrue << endl;

//		cout << " true count, false count " << trueCount << " " << falseCount << endl;
		featIter++;
	}
}


string Bayes::pickTraining(int against) {
	string against0 = "1, 2, 3, 4";
	string against1 = "0, 2, 3, 4";
	string against2 = "0, 1, 3, 4";
	string against3 = "0, 1, 2, 4";
	string against4 = "0, 1, 2, 3";

	if (against == 0) {
		return against0;
	} else if (against == 1) {
		return against1;
	} else if (against == 2) {
		return against2;
	} else if (against == 3) {
		return against3;
	} else if (against == 4) {
		return against4;
	}
	return "";
}

void Bayes::crossValidate(double smoothness) {
	//train on 0, 1, 2, 3, test against 4 etc.
	cout << "\n\nCross validating with smoothness = " << smoothness << endl;
	string training;

	for (int against = 0; against < trainingFiles.size(); against++) {

		training = pickTraining(against);
		cout << "Training on files: " << training
				<< " and testing against file: " << trainingFiles.at(against)
				<< endl;

		for (int train = 0; train < trainingFiles.size(); train++) {

			if (train != against) {

				stream(trainingFiles.at(train), false); //training map consists of the entire file now with positives.

			}

		}
		computeProbabilityTable(smoothness);
		test(trainingFiles.at(against));
		averagePercentage += percentageCross;
		averageOfCross += percentageCross;
		numberOfCross++;

		//reset
		trainingMap.clear();
		labels.clear();
		probabilityTable.clear();
//		weights.clear();
//		gamma_t = 0;

	}

	//	cout << averagePercentage << " = sum. " << endl;
	averagePercentage = averagePercentage / (trainingFiles.size());
	//	cout << "Average percentage " << averagePercentage << endl;

}
void Bayes::test(string filepath) {
	stream(filepath, true);
}
void Bayes::go() {
//	stream(trainingFiles.at(0), false);
//	cout << "size of training map: " << trainingMap.size() << endl;
//	cout << "size of labels " << labels.size() << endl;
//	cout << "featuredmentioend size " << featuresMentioned.size() << endl;

	prior = numLabelTrue / labels.size(); //CHECK THIS BEFORE CHANING FILES

	for (int smooth = 0; smooth < smoothness.size(); smooth++) {
		crossValidate(smoothness.at(smooth));
		cout << "Average percentage for 5 fold validation with a rate of "
				<< smoothness.at(smooth) << " = " << averagePercentage << endl;

		pair<double, double> p;
		p.first = smoothness.at(smooth);
//		p.second = tradeoff.at(sigma);

		accuracyPoints.insert(make_pair(p, averagePercentage));
		averagePercentage = 0;
		percentageCross = 0;
	}

	double maxAccuracy = 0;
	pair<double, double> optimizedHyperParams;
	for (map<pair<double, double>, double>::iterator iter =
			accuracyPoints.begin(); iter != accuracyPoints.end(); iter++) {
		if (iter->second > maxAccuracy) {
			maxAccuracy = iter->second;
			optimizedHyperParams.first = iter->first.first;
			optimizedHyperParams.second = iter->first.second;
		}
	}

	cout << "Optimized hyper params: smoothness = " << optimizedHyperParams.first << endl;

	averageOfCross /= numberOfCross;

	cout << "Average accuracy during cross validation:  " << averageOfCross
			<< endl;
	cout << "Running optimized hyper params on speeches train and test file"
			<< endl;

	stream(testingFiles.at(0), false); //training map consists of the entire file now with positives.
	computeProbabilityTable(optimizedHyperParams.first);
	test(testingFiles.at(1));
	cout << "\nAccuracy with optimal hyper params: " << percentageCross << endl;
	cout << "\n***" << endl;

}
