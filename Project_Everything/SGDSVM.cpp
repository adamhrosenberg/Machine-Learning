/*
 * SGDSVM.cpp
 *
 *  Created on: Nov 24, 2017
 *      Author: adam
 */

#include "SGDSVM.h"

using namespace std;
SGDSVM::SGDSVM() {
//	= {"data-splits/data.test"}
	trainingFiles.push_back("data-splits/data.train");
	testingFiles.push_back("data-splits/data.test");
//	testingFiles.push_back("data-splits/data.eval.anon");

	//	vector<double> rates = {10, 1, 0.1, 0.01, 0.001, 0.0001}; //4
	//	vector<double> tradeoff = {10, 1, 0.1, 0.01, 0.001, 0.0001}; //2

	rates.push_back(.01);
	tradeoff.push_back(1);
	// TODO Auto-generated constructor stub

}

SGDSVM::~SGDSVM() {
	// TODO Auto-generated destructor stub
}

double SGDSVM::dotP(map<double, double> row, map<double, double> weights) {
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

void SGDSVM::shuffle() {
	for (int line = labels.size() - 1; line > 0; line--) {
		srand(time(NULL));
//		srand(55);

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

void SGDSVM::scale(double scalar, map<double, double> * vector) {
	map<double, double>::iterator iter = vector->begin();
	while (iter != vector->end()) {
		iter->second *= scalar;
		iter++;
	}
}

void SGDSVM::run(double rate, double tradeoff) {
	shuffle();
	for (int row = 0; row < labels.size(); row++) {

		double result = dotP(trainingMap.at(row), weights);

		if (labels.at(row) * result <= 1) {
//			cout << "LESS"<<endl;
//			gamma_t = rate / (1 + (rate * row / tradeoff));
			gamma_t = rate / (1 + tradeoff);

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
				map<double, double>::iterator search = trainingMap.at(row).find(index);
				if (search != trainingMap.at(row).end()) {
//					witer->second += trainingMap.at(row).find(index)->second;
					witer->second += search->second;
				} else {
					count++;
				}
				witer++;
			}
		} else {
			gamma_t = rate / (1 + tradeoff);
			scale(1 - gamma_t, &weights);
		}
	}
}

void SGDSVM::stream(string filepath, bool isTest) {
	ifstream pipein(filepath.c_str());
	double right = 0, wrong = 0;
	for (string line; getline(pipein, line);) {
		map<double, double> example;
//		cout << line.at(0);
		double label = ((line.at(0) == '0') ? -1 : 1);
//		labels.push_back(label);
//		cout << label<<endl;
		line = line.substr(2); //2 was og 1
		std::string token;
		std::istringstream tokenStream(line);
		float key;
		while (std::getline(tokenStream, token, ' ')) {
			key = atof(token.c_str());
			int colonIndex = 0;
//			for(int letter = 0 ; letter < token.length(); letter++){
//				if(token.at(letter) == ":"){
//					colonIndex = letter;
//				}
//			}
//
//			string latter = token.substr()
			size_t pos = token.find(":");      // position of "live" in str

			string str3 = token.substr(pos+1);     // get from "live" to the end

			double featValue = stod(str3);

			example.insert(make_pair(key, featValue));
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
		cout << "\tAccuracy " << percentageCross << endl;
//		cout << "Right: " << right << " wrong: " << wrong << endl;
	}
}

void SGDSVM::test(string filepath) {
	stream(filepath, true);
}

string SGDSVM::pickTraining(int against) {
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
void SGDSVM::crossValidate(double rate, double tradeoff) {
}
void SGDSVM::go() {

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
//            cout << userIDs.at(i) << "," << guess << endl;
    }



}
