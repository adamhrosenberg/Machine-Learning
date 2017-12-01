/*
 * ID3.cpp
 *
 *  Created on: Dec 1, 2017
 *      Author: adam
 */

#include "ID3.h"

ID3::ID3() {
	// TODO Auto-generated constructor stub

}

ID3::~ID3() {
	// TODO Auto-generated destructor stub
}

void ID3::stream(string filepath, bool isTest) {
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
				featuresMentioned.insert(key);
			}
			if (!isTest) {
				labels.push_back(label);
				if(label != 1 && allLabelsTrue){
					allLabelsTrue = false;
				}
//				weights[key] = 0;
				trainingMap.push_back(example);
			} else {
//				double dot = dotP(example, weights);
				double temp = 0;
//				if (dot < 0) {
//					temp = -1;
//				} else {
//					temp = 1;
//				}
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
//			percentageCross = right / (right + wrong);
			cout << "Accuracy " << right / (right + wrong) << endl;
	//		cout << "Right: " << right << " wrong: " << wrong << endl;
		}
}
void ID3::test(string filepath) {

}

double ID3::calculateEntropy(vector<map<double, double>> S){

	return 0;
}

double ID3::calculateGain(vector<map<double, double>> S, set<int> attributes, double totalEntropy){

	return 0;
}


bool ID3::seeIfAllTrue(vector<double> l){
	for(int i = 0; i < l.size(); i++){
		if(l.at(i) == -1){
			return false;
		}
	}
	return true;
}

int ID3::bestAttributeThatClassifiesS(){


}
TreeNode ID3::recurse(vector<map<double, double>> S, set<int> attributes, vector<double> l){

	if(seeIfAllTrue(l)){
		TreeNode root;
		root.leaf = true;
		root.label_value = true;
		return root;
	}

	cout << "else"<<endl;

	TreeNode root;

	vector<map<double, double>> S_v;

	int A = calculateEntropy();

	for(int v = 0; v < possibleValuesForA; v++){

	}

	return root;


}

void ID3::run(int depth) {

	vector<map<double, double>> S = trainingMap;
	vector<double> l = labels;
	set<int> attributes = featuresMentioned;

	recurse(S, attributes, l);
}

void ID3::go() {
	stream(trainingFiles.at(0), false); //training map consists of the entire file now with positives.
	run(3);
//	test(trainingFiles.at(against));

	cout << "done";

}
