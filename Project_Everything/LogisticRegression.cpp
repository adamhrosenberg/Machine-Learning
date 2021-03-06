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

			size_t pos = token.find(":");      // position of "live" in str

						string str3 = token.substr(pos+1);     // get from "live" to the end

						double featValue = stod(str3);

						example.insert(make_pair(key, featValue));
//			example.insert(make_pair(key, 1));
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

			predictions.push_back(temp);

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
void LogisticRegression::shuffle() {
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

void LogisticRegression::scale(double scalar, map<double, double> * vector) {
//	cout << "Scaling " << endl;
	map<double, double>::iterator iter = vector->begin();
	while (iter != vector->end()) {
		iter->second *= scalar;
		iter++;
	}
}
LogisticRegression::LogisticRegression() {
	// TODO Auto-generated constructor stub

	trainingFiles.push_back("data-splits/data.train");
//	testingFiles.push_back("data-splits/data.test");
	testingFiles.push_back("data-splits/data.eval.anon");

	vector<double> rates = {10, 1, 0.1, 0.01, 0.001, 0.0001}; //4
	vector<double> tradeoff = {10, 1, 0.1, 0.01, 0.001, 0.0001}; //2

	gamma_t = 0;
}

LogisticRegression::~LogisticRegression() {
	// TODO Auto-generated destructor stub
}

void LogisticRegression::updateWeights(double rate, double tradeoff) {

}

map<double, double> LogisticRegression::mapAdd(map<double, double> * weights,
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
void LogisticRegression::run(double rate, double tradeoff) {
//	cout << "Training with rate: " << rate << " tradeoff = " << tradeoff
//			<< endl;
	shuffle();
	t = 0;
	for (int row = 0; row < labels.size(); row++) {
//		cout << "training on row: " << row << endl;
		gamma_t = rate / (1 + (rate * t / sqrt(tradeoff)));

//		gamma_t = rate / (1 + t);

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

void LogisticRegression::test(string filepath) {
	stream(filepath, true);
}

string LogisticRegression::pickTraining(int against) {
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
void LogisticRegression::crossValidate(double rate, double tradeoff) {

	//train on 0, 1, 2, 3, test against 4 etc.
	cout << "\n\nCross validating with rate = " << rate << " and tradeoff ^2 = "
			<< tradeoff << endl;
	string training;

//	for (int against = 0; against < trainingFiles.size(); against++) {



//		training = pickTraining(against);
//		cout << "Training on files: " << training
//				<< " and testing against file: " << trainingFiles.at(against)
//				<< endl;

//		for (int train = 0; train < trainingFiles.size(); train++) {
//
//			if (train != against) {

				stream(trainingFiles.at(0), false); //training map consists of the entire file now with positives.

//			}
//
//		}
		run(rate, tradeoff);
		test(testingFiles.at(0));
		averagePercentage += percentageCross;
		averageOfCross += percentageCross;
		numberOfCross ++ ;

		//reset
		trainingMap.clear();
		labels.clear();
		weights.clear();
		gamma_t = 0;

//	}

//	cout << averagePercentage << " = sum. " << endl;
//	averagePercentage = averagePercentage / (trainingFiles.size());
//	cout << "Average percentage " << averagePercentage << endl;
}
void LogisticRegression::go() {

	//writing file for kaggle
	stream(trainingFiles.at(0), false); //training map consists of the entire file now with positives.
	run(rates.at(0), tradeoff.at(0));

	test(testingFiles.at(0));

    vector<string> userIDs;
//      ifstream pipein();

    ifstream pipein("data-splits/data.eval.id");

    //read in the file.
    for (string line; getline(pipein, line);) {
            //now that we have a line, we need to create a vector. first value in examples is the label. then its each value of the attributes.
//              cout << line << endl;
            userIDs.push_back(line);
    }

    cout << "Id,Prediction" << endl;

    for (int i = 0; i < predictions.size(); i++) {
            int guess = (predictions.at(i) == -1) ? 0 : 1;
            cout << userIDs.at(i) << "," << guess << endl;
    }

//		for (int rate = 0; rate < rates.size(); rate++) {
//				for (int sigma = 0; sigma < tradeoff.size(); sigma++) {
//					crossValidate(rates.at(rate), tradeoff.at(sigma));
//					cout << "Average percentage for 5 fold validation with a rate of "
//							<< rates.at(rate) << " and a sigma of "
//							<< tradeoff.at(sigma) << " = " << averagePercentage << endl;
//
//					pair<double, double> p;
//					p.first = rates.at(rate);
//					p.second = tradeoff.at(sigma);
//
//					accuracyPoints.insert(make_pair(p, averagePercentage));
//					averagePercentage = 0;
//					percentageCross = 0;
//				}
//			}
//
//			double maxAccuracy = 0;
//			pair<double, double> optimizedHyperParams;
//			for (map<pair<double, double>, double>::iterator iter = accuracyPoints.begin();
//					iter != accuracyPoints.end(); iter++) {
//				if (iter->second > maxAccuracy) {
//					maxAccuracy = iter->second;
//					optimizedHyperParams.first = iter->first.first;
//					optimizedHyperParams.second = iter->first.second;
//				}
//			}
//
//			cout << "Optimized hyper params: gamma = " << optimizedHyperParams.first
//					<< " and sigma = " << optimizedHyperParams.second << endl;
//
//
//			averageOfCross /= numberOfCross;
//
//			cout << "Average accuracy during cross validation:  " << averageOfCross << endl;
//			cout << "Running optimized hyper params on speeches train and test file"
//					<< endl;
//
//
//			stream(trainingFiles.at(0), false); //training map consists of the entire file now with positives.
//			run(optimizedHyperParams.first, optimizedHyperParams.second);
//
//			test(testingFiles.at(0));
//			cout << "\nAccuracy with optimal hyper params: " << percentageCross << endl;
//			cout << "\n***" << endl;

}

