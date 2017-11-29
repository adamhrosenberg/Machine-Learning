/*
 * LogisticRegression.cpp
 *
 *  Created on: Nov 28, 2017
 *      Author: adam
 */

#include "LogisticRegression.h"

using namespace std;

double LogisticRegression::dotP(map<double, double> row,
		map<double, double> weights) {
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
void LogisticRegression::stream(string filepath, bool isTest) {
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
			cout << "guess: " << temp << " label: " << label << " dot: " << dot
					<< endl;

			if (temp == label) {
				right++;
			} else {
				wrong++;
			}
		}
	}
	if (isTest) {
		cout << "Accuracy " << right / (right + wrong) << endl;
		cout << "Right: " << right << " wrong: " << wrong << endl;
	}
}
void LogisticRegression::shuffle() {
	cout << "Shuffling" << endl;
	for (int line = labels.size() - 1; line > 0; line--) {
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

void LogisticRegression::scale(double scalar, map<double, double> * vector) {
	cout << "Scaling " << endl;
	map<double, double>::iterator iter = vector->begin();
	while (iter != vector->end()) {
		iter->second *= scalar;
		iter++;
	}
}
LogisticRegression::LogisticRegression() {
	// TODO Auto-generated constructor stub

}

LogisticRegression::~LogisticRegression() {
	// TODO Auto-generated destructor stub
}

void LogisticRegression::updateWeights(double rate, double tradeoff) {

}

void LogisticRegression::mapAdd(map<double, double> * first, map<double, double> * second){
	cout << "Adding " << endl;
	map<double, double>::iterator firstIter = first->begin();
	map<double, double>::iterator secondIter = second->begin();

	while(firstIter != first->end()){
		cout << "first: " << firstIter->second << " second " << secondIter->second << endl;
		firstIter->second = firstIter->second + secondIter->second;
		secondIter++;
		firstIter++;
	}

}
void LogisticRegression::run(double rate, double tradeoff) {
	shuffle();
	for (int row = 0; row < labels.size(); row++) {
		cout << "training on row: " << row << endl;
		double result = dotP(trainingMap.at(row), weights);
		gamma_t = rate / (1 + (rate * row / tradeoff));

		map<double, double> example = trainingMap.at(row);
		scale(-1 * labels.at(row), &example);
		double dotResult = dotP(example, weights);
		double expo = labels.at(row) * dotResult;
		double exponentiation = exp(expo);
		scale(1 / (1 + exponentiation), &example);

		scale(2 * tradeoff, &weights);

		mapAdd(&example, &weights); //exmaple is now equals to the sum.
		scale(-1 * rate, &example);
		mapAdd(&weights, &example);
	}
}

void LogisticRegression::test(string filepath) {
	stream(filepath, true);
}

void LogisticRegression::go() {
	stream(trainingFiles.at(0), false);
	run(.1, 100);
	cout << "done" << endl;
//	test("data/speeches.test.liblinear");
}

