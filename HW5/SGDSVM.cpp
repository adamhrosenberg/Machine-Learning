/*
 * SGDSVM.cpp
 *
 *  Created on: Nov 24, 2017
 *      Author: adam
 */

#include "SGDSVM.h"

using namespace std;
SGD_SVM::SGD_SVM() {
	Files f;
	trainingFiles = f.trainingFiles;
	testingFiles = f.testingFiles;
	// TODO Auto-generated constructor stub

}

SGD_SVM::~SGD_SVM() {
	// TODO Auto-generated destructor stub
}

double SGD_SVM::dotP(map<double, double> row, map<double, double> weights) {
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

void SGD_SVM::shuffle() {
	for (int line = labels.size() - 1; line > 0; line--) {
//		srand(time(NULL));
		srand(31);

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

void SGD_SVM::scale(double scalar, map<double, double> * vector) {
	map<double, double>::iterator iter = vector->begin();
	while (iter != vector->end()) {
		iter->second *= scalar;
		iter++;
	}
}

void SGD_SVM::run(double rate, double tradeoff) {
	shuffle();
	for (int row = 0; row < labels.size(); row++) {

		double result = dotP(trainingMap.at(row), weights);

		if (labels.at(row) * result <= 1) {
//			cout << "LESS"<<endl;
			gamma_t = rate / (1 + (rate * row / tradeoff));
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
//					witer->second += trainingMap.at(row).find(index)->second;
					witer->second += search->second;
				} else {
					count++;
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

void SGD_SVM::stream(string filepath, bool isTest) {
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
//		cout << "\tAccuracy " << percentageCross << endl;
//		cout << "Right: " << right << " wrong: " << wrong << endl;
	}
}

void SGD_SVM::test(string filepath) {
	stream(filepath, true);
}

string SGD_SVM::pickTraining(int against) {
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
void SGD_SVM::crossValidate(double rate, double tradeoff) {
	//train on 0, 1, 2, 3, test against 4 etc.
	cout << "\n\nCross validating with rate = " << rate << " and tradeoff ^2 = "
			<< tradeoff << endl;
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
		run(rate, tradeoff);
		test(trainingFiles.at(against));
		averagePercentage += percentageCross;
		averageOfCross += percentageCross;
		numberOfCross ++ ;

		//reset
		trainingMap.clear();
		labels.clear();
		weights.clear();
		gamma_t = 0;

	}

//	cout << averagePercentage << " = sum. " << endl;
	averagePercentage = averagePercentage / (trainingFiles.size());
//	cout << "Average percentage " << averagePercentage << endl;
}
void SGD_SVM::go() {

	for (int rate = 0; rate < rates.size(); rate++) {
		for (int sigma = 0; sigma < tradeoff.size(); sigma++) {
			crossValidate(rates.at(rate), tradeoff.at(sigma));
			cout << "Average percentage for 5 fold validation with a rate of "
					<< rates.at(rate) << " and a sigma of "
					<< tradeoff.at(sigma) << " = " << averagePercentage << endl;

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
	cout << "Running optimized hyper params on speeches train and test file"
			<< endl;


	stream(testingFiles.at(0), false); //training map consists of the entire file now with positives.
	run(optimizedHyperParams.first, optimizedHyperParams.second);

	test(testingFiles.at(1));
	cout << "\nAccuracy with optimal hyper params: " << percentageCross << endl;
	cout << "\n***\n" << endl;


}
