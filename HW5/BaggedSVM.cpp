/*
 * baggedsvm.cpp
 *
 *  Created on: Nov 24, 2017
 *      Author: adam
 */

#include "BaggedSVM.h"

using namespace std;
BaggedSVM::BaggedSVM() {
	// TODO Auto-generated constructor stub

}

BaggedSVM::~BaggedSVM() {
	// TODO Auto-generated destructor stub
}

double BaggedSVM::dotP(map<double, double> row, map<double, double> weights) {
	double result = 0;
	map<double, double>::iterator iter = row.begin();
	while (iter != row.end()) {
		double key = iter->first;
		double value = iter->second;
		double weight = weights[key];
		result += value * weight;
		iter++;
	}
//	cout << result << endl;
	return result;
}

void BaggedSVM::shuffle() {
	for (int line = labels.size() - 1; line > 0; line--) {
//		srand(time(NULL));
		srand(50);

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

void BaggedSVM::scale(double scalar, map<double, double> * vector) {
	map<double, double>::iterator iter = vector->begin();
	while (iter != vector->end()) {
		iter->second *= scalar;
		iter++;
	}
}

void BaggedSVM::run(double rate, double tradeoff) {

	for (int row = 0; row < labels.size(); row++) {
//		cout << "row " << row << endl;

		double result = dotP(trainingMap.at(row), weights);

		if (labels.at(row) * result <= 1) {
//			gamma_t = rate / (1 + (rate * row / tradeoff));
			gamma_t = rate/ (1 + tradeoff);
			//first term

			scale(1 - gamma_t, &weights);

			//second term
			scale(tradeoff, &trainingMap.at(row));
			scale(labels.at(row), &trainingMap.at(row));
			scale(gamma_t, &trainingMap.at(row));

			//now see if there is a corresponding true feature value
			map<double, double>::iterator witer = weights.begin();
			while (witer != weights.end()) {
				double index = witer->first;
				auto search = trainingMap.at(row).find(index);
				if (search != trainingMap.at(row).end()) {
					witer->second += search->second;
				}
				witer++;
			}
		} else {
//			cout << "GREATER" << endl;
			gamma_t = rate / (1 + (rate * row / tradeoff));
			scale(1 - gamma_t, &weights);
		}
	}
}

void BaggedSVM::stream(string filepath, bool isTest) {
	ifstream pipein(filepath.c_str());
	double right = 0, wrong = 0;
	for (string line; getline(pipein, line);) {
		map<double, double> example;
		double label = ((line.at(0) == '-') ? -1 : 1);
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
			if (temp == label) {
				right++;
			} else {
				wrong++;
			}
		}
	}
	if (isTest) {

		percentageCross = right / (right + wrong);
	}
}

void BaggedSVM::test(string filepath) {
//	stream(filepath, true);
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

	count++;
	percentageCross = right / (right + wrong);

}

string BaggedSVM::pickTraining(int against) {
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
void BaggedSVM::crossValidate(double rate, double tradeoff) {
	//train on 0, 1, 2, 3, test against 4 etc.
	cout << "Cross validating with gamma = " << rate << " and C = " << tradeoff
			<< endl;

	run(rate, tradeoff);
	test(trainingFiles.at(1)); //doesnt matter what i pass it. test is soelf contained
	averagePercentage += percentageCross;
	averageOfCross += percentageCross;
	numberOfCross++;
}
void BaggedSVM::go() {

//	run(.1, .1);

	for (int rate = 0; rate < rates.size(); rate++) {
		for (int sigma = 0; sigma < tradeoff.size(); sigma++) {
			crossValidate(rates.at(rate), tradeoff.at(sigma));

			pair<double, double> p;
			p.first = rates.at(rate);
			p.second = tradeoff.at(sigma);
//			cout << "Average percentage for 5 fold validation with a rate of "
//								<< rates.at(rate) << " and a sigma of "
//								<< tradeoff.at(sigma) << " = " << averagePercentage << endl;
			accuracyPoints.insert(make_pair(p, averagePercentage));
			averagePercentage = 0;
			percentageCross = 0;
		}
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

	cout << "Optimized hyper params: gamme = " << optimizedHyperParams.first
			<< " and C = " << optimizedHyperParams.second << endl;

	averageOfCross /= numberOfCross;

	cout << "Average accuracy during cross validation:  " << averageOfCross
			<< endl;
	cout << "Running optimized hyper params on speeches train and test file"
			<< endl;

	stream("data/speeches.train.liblinear", false); //training map consists of the entire file now with positives.
	run(optimizedHyperParams.first, optimizedHyperParams.second);

	test("data/speeches.test.liblinear");
	cout << "\n***\nAccuracy with optimal hyper params: " << percentageCross
			<< endl;

}
