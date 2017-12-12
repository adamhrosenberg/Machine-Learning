/*
 * Perceptron.cpp
 *
 *  Created on: Dec 11, 2017
 *      Author: adam
 */

#include "Perceptron.h"

Perceptron::Perceptron() {
	for(int i = 0; i < 17; i++){
		weights.push_back(0);
	}
	// TODO Auto-generated constructor stub

}

Perceptron::~Perceptron() {
	// TODO Auto-generated destructor stub
}


void Perceptron::update(vector<double> example, double r) {
	double trainingY = example.at(0);
	for (unsigned int i = 1; i < example.size(); i++) {
		double weight = weights.at(i) + (r * trainingY);
		weights.at(i) = weight;
	}
}

double Perceptron::dotP(vector<double> weights, vector<double> row) {
	double result = 0;
	for (unsigned int i = 0; i < row.size(); i++) {
		result += weights.at(i) * row.at(i);
	}
	return result;
}

double Perceptron::perceptron(vector<double> example, bool isTest) {
	double r = 0.1;
	example.erase(example.begin() + 1); //remove extra labeling.

	double prediction = (dotP(weights, example) > 0) ? 1 : -1;
	//if prediction is true, then it is > 0, false is <= 0.
//	cout << example.at(0) << " , " << prediction << endl;
	if (example.at(0) != prediction && !isTest) {
		//made a mistake.
		update(example, 1);
	}
	if (isTest)
		predictions.push_back(prediction);
	return prediction;
}

void Perceptron::stream(string filepath, bool isTest) {
//	string filepath = "Dataset/data-splits/data.train"; //store path since the pipe consumes that string.
	ifstream pipein(filepath.c_str());

	double right, wrong = 0;
	//read in the file.
	for (string line; getline(pipein, line);) {
		vector<double> example;
		//now that we have a line, we need to create a vector. first value in examples is the label. then its each value of the attributes.
		double label = ((line.at(0) == '0') ? -1 : 1);
		example.push_back(label);
		line = line.substr(1);

		std::vector<std::string> tokens;
		std::string token;
		std::istringstream tokenStream(line);
		while (std::getline(tokenStream, token, ':')) {
			example.push_back(atof(token.c_str()));
		}

		//label == perceptron
		double perc = perceptron(example, isTest);
		if (perc == label) {
			//guessed right.
			right++;
		} else {
			wrong++;
		}

	}

//	cout << "**\nFor file: " << filepath << " the percentage is " << right / (right + wrong) << " num right: " << right << " num wrong: " << wrong << endl;
}

void Perceptron::populateWeights() {
	srand(time(NULL));
	for (unsigned int i = 0; i < weights.size(); i++) {
		double randomInitValue = rand() % (20000) - (10000);
		randomInitValue /= 10000;
		weights.at(i) = (randomInitValue);
	}
}

void Perceptron::go() {
	populateWeights();
	//start perceptron on training file.
	stream("data-splits/data.train", false); //training
	stream("data-splits/data.eval.anon", true); //testing
//	stream("Dataset/data-splits/data.test", true); //testing

	vector<string> userIDs;
//	ifstream pipein();

	ifstream pipein("data-splits/data.eval.id");

	//read in the file.
	for (string line; getline(pipein, line);) {
		//now that we have a line, we need to create a vector. first value in examples is the label. then its each value of the attributes.
//		cout << line << endl;
		userIDs.push_back(line);
	}

	cout << "Id,Prediction" << endl;

	for (int i = 0; i < predictions.size(); i++) {
		int guess = (predictions.at(i) == -1) ? 0 : 1;
		cout << userIDs.at(i) << "," << guess << endl;
	}
//	return 0;
}
