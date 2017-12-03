/*
 * BaggedLogistic.cpp
 *
 *  Created on: Dec 3, 2017
 *      Author: adam
 */

#include "BaggedLogistic.h"


using namespace std;

double BaggedLogistic::dotP(map<double, double> row,
		map<double, double> weights) {
	double result = 0;
	map<double, double>::iterator iter = row.begin();
	while (iter != row.end()) {
		double key = iter->first;
		double value = iter->second;



		map<double, double>::iterator findIter = weights.begin();
		findIter = weights.find(iter->first);
		if (findIter != weights.end()) {
//			cout << "Find iter first: " << findIter->first << endl;
			double weight = weights[key];
			result += value * weight;
		}


		iter++;
	}
//	cout << result << endl;
	return result;
}
void BaggedLogistic::stream(string filepath, bool isTest) {
	ifstream pipein(filepath.c_str());
	double right = 0, wrong = 0;
	for (string line; getline(pipein, line);) {
		map<double, double> example;
		double label = ((line.at(0) == '-') ? -1 : 1);
//		labels.push_back(label);
		line = line.substr(2); //2 was og 1
		std::string token;
		std::istringstream tokenStream(line);
		float key;
		while (std::getline(tokenStream, token, ' ')) {
			key = atof(token.c_str());
			example.insert(make_pair(key, 1));
		}
		if (!isTest) {
			labels.push_back(label);
			weights[key] = 0;
			trainingMap.push_back(example);
		} else {
			double dot = dotP(example, weights);
			double temp = 0;
			if (dot < 0) {
				temp = -1;
			} else {
				temp = 1;
			}
//			cout << "guess: " << temp << " label: " << label << " dot: " << dot
//					<< endl;

			if (temp == label) {
				right++;
			} else {
				wrong++;
			}
		}
	}
	if (isTest) {
		percentageCross = right / (right + wrong);
//		cout << "Accuracy " << right / (right + wrong) << endl;
//		cout << "Right: " << right << " wrong: " << wrong << endl;
	}
}
void BaggedLogistic::shuffle() {
//	cout << "Shuffling" << endl;
	for (int line = labels.size() - 1; line > 0; line--) {
//		srand(time(NULL));
		srand(30);

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

void BaggedLogistic::scale(double scalar, map<double, double> * vector) {
//	cout << "Scaling " << endl;
	map<double, double>::iterator iter = vector->begin();
	while (iter != vector->end()) {
		iter->second *= scalar;
		iter++;
	}
}
BaggedLogistic::BaggedLogistic() {
	// TODO Auto-generated constructor stub
	Files f;
	trainingFiles = f.trainingFiles;
	testingFiles = f.testingFiles;

}

BaggedLogistic::~BaggedLogistic() {
	// TODO Auto-generated destructor stub
}

void BaggedLogistic::updateWeights(double rate, double tradeoff) {

}

map<double, double> BaggedLogistic::mapAdd(map<double, double> * weights,
		map<double, double> * example) {
//	cout << "Adding " << endl;
	map<double, double> result = *weights;

	map<double, double>::iterator witer = weights->begin();
	map<double, double>::iterator xiter = example->begin();

	for (; xiter != example->end(); xiter++) {

		map<double, double>::iterator findIter = weights->begin();
		findIter = weights->find(xiter->first);
		if (findIter != weights->end()) {
//			cout << "Find iter first: " << findIter->first << endl;
			result[findIter->first] = xiter->second + findIter->second;
		}
	}
	return result;
//	cout << "end add" << endl;

}
void BaggedLogistic::run(double rate, double tradeoff) {
//	cout << "Training with rate: " << rate << " tradeoff = " << tradeoff
//			<< endl;
	shuffle();
	t = 0;
	for (int row = 0; row < labels.size(); row++) {
//		cout << "training on row: " << row << endl;
//		gamma_t = rate / (1 + (rate * t / sqrt(tradeoff)));

		gamma_t = rate / (1 + t);

		map<double, double> mid; //large paran value
		map<double, double> origW = weights;
		map<double, double> example = trainingMap.at(row);

		map<double, double> rightW = weights;
		map<double, double> leftW = weights;
		map<double, double> exampleTop = trainingMap.at(row);
		map<double, double> exampleBottom = trainingMap.at(row);

		scale(2 / (sqrt(tradeoff)) , &rightW);


		double dot = labels.at(row) * dotP(exampleBottom, leftW);
		double exponentiation = exp(dot);
		scale((-1 * labels.at(row)) / (1 + exponentiation), &exampleTop);

		mid = mapAdd(&rightW, &exampleTop);

		scale(-1 * gamma_t, &mid);
		weights = mapAdd(&origW, &mid);

		t+= 1;
	}
}

void BaggedLogistic::test(string filepath) {

	double right = 0, wrong = 0;
	for (int row = 0; row < trainingMap.size(); row++) {
		map<double, double> example = trainingMap.at(row);
		double label = labels.at(row);
		double dot = dotP(example, weights);

		double temp = 0;
		if (dot < 0) {
			temp = -1;
		} else {
			temp = 1;
		}

		if (temp == label) {
			right++;
		} else {
			wrong++;
		}
//					cout << "guess: " << temp << " label: " << label << " dot: " << dot
//							<< endl;
	}

	percentageCross = right / (right + wrong);
}

string BaggedLogistic::pickTraining(int against) {
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
void BaggedLogistic::crossValidate(double rate, double tradeoff) {

	//train on 0, 1, 2, 3, test against 4 etc.
	cout << "Cross validating with rate = " << rate << " and tradeoff ^2 = "
			<< tradeoff << endl;
	string training;


		run(rate, tradeoff);
		test(trainingFiles.at(1));
		averagePercentage += percentageCross;
		averageOfCross += percentageCross;
		numberOfCross ++ ;

		//reset
//		trainingMap.clear();
//		labels.clear();
//		weights.clear();
//		gamma_t = 0;


//	cout << averagePercentage << " = sum. " << endl;
//	averagePercentage = averagePercentage / (trainingFiles.size());
//	cout << "Average percentage " << averagePercentage << endl;
}
void BaggedLogistic::go() {

		for (int rate = 0; rate < rates.size(); rate++) {
				for (int sigma = 0; sigma < tradeoff.size(); sigma++) {
					crossValidate(rates.at(rate), tradeoff.at(sigma));

					pair<double, double> p;
					p.first = rates.at(rate);
					p.second = tradeoff.at(sigma);

					accuracyPoints.insert(make_pair(p, averagePercentage));
					averagePercentage = 0;
					percentageCross = 0;
				}
			}

			double maxAccuracy = 0;
			pair<double, double> optimizedHyperParams;
			for (map<pair<double, double>, double>::iterator iter = accuracyPoints.begin();
					iter != accuracyPoints.end(); iter++) {
				if (iter->second > maxAccuracy) {
					maxAccuracy = iter->second;
					optimizedHyperParams.first = iter->first.first;
					optimizedHyperParams.second = iter->first.second;
				}
			}

			cout << "Optimized hyper params: delta = " << optimizedHyperParams.first
					<< " and sigma = " << optimizedHyperParams.second << endl;


			averageOfCross /= numberOfCross;

			cout << "Average accuracy during cross validation:  " << averageOfCross << endl;
			cout << "Running optimized hyper params"
					<< endl;


			stream(testingFiles.at(0), false); //training map consists of the entire file now with positives.
			run(optimizedHyperParams.first, optimizedHyperParams.second);

			test(testingFiles.at(1));
			cout << "\nAccuracy with optimal hyper params: " << percentageCross << endl;
			cout << "\n***" << endl;

}

