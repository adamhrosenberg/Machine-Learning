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


bool ID3::seeIfAllTrue(vector<double> l){
	for(int i = 0; i < l.size(); i++){
		if(l.at(i) == -1){
			return false;
		}
	}
	return true;
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

double ID3::calculateTotalEntropy(vector<double> l){
	cout << "calculating total entropy " << endl;
	double totalEntropy = 0;
	double posCount = 0;

	for(int label = 0 ; label < l.size(); label++){
		if(l.at(label) == 1){
			posCount ++;
		}
	}

	double proportion = posCount / l.size();

	double posPro = proportion;
	double negPro = 1 - (proportion);


	totalEntropy = (-1 * posPro * log2(posPro)) - (negPro * log2(negPro));

	return totalEntropy;
}
double ID3::calculateEntropy(vector<map<double, double>> S, vector<double> l, int featureNumber){
	cout << "Calculating entropy for feat # " << featureNumber << endl;
	double expectedEntropy = 0;

	double yesEntropy = 0;
	double noEntropy = 0;

	double yesPosCount = 0;
	double yesNegCount = 0;

	double noPosCount = 0;
	double noNegCount = 0;

	for(int row = 0; row < S.size(); row++){
		map<double, double>::iterator sIter;
		sIter = S.at(row).find(featureNumber);



		if(sIter != S.at(row).end()){
			//the feature is there and TRUE
			if(l.at(row) == 1){
				yesPosCount ++ ;
			}else{
				yesNegCount ++;
			}
		}else{
			//not there. value if negative.
			if(l.at(row) == 1){
				noPosCount ++ ;
			}else{
				noNegCount ++;
			}
		}
	}

	double yesTotal = yesPosCount + noNegCount;

	yesEntropy = (-1 * (yesPosCount / yesTotal) * log2(yesPosCount / yesTotal) -
					   (yesNegCount / yesTotal) * log2(yesNegCount / yesTotal));

	double noTotal = noPosCount + noNegCount;
	noEntropy = (-1 * (noPosCount / noTotal) * log2(noPosCount / noTotal) -
				  	  (noNegCount / noTotal) * log2(noNegCount / noTotal));


	double yesCoef = yesTotal / l.size();
	double noCoef = noTotal / l.size();

	expectedEntropy = (yesCoef * yesEntropy) + (noCoef * noEntropy);

	return expectedEntropy;
}

double ID3::calculateGain(vector<map<double, double>> S, vector<double> l, double totalEntropy, int featureNumber){

//	double calculateEntropy(vector<map<double,double>> S, vector<double> l, int featureNumber);

	return totalEntropy - calculateEntropy(S, l, featureNumber);
}



int ID3::bestAttributeThatClassifiesS(vector<map<double, double>> S, set<int> attributes, vector<double> l,double totalEntropy){

	double maxGain = 0;
	double maxFeat = 0;

	//calculate gain for every feature.
	for(set<int>::iterator attIter = attributes.begin(); attIter != attributes.end(); attIter++){

		double gain = calculateGain(S, l, totalEntropy, *attIter);
		if(gain > maxGain){
			maxGain = gain;
			maxFeat = *attIter;
		}
	}
	return maxFeat;
}

TreeNode ID3::recurse(vector<map<double, double>> S, set<int> attributes, vector<double> l){

	if(seeIfAllTrue(l)){
		TreeNode root;
		root.leaf = true;
		root.label_value = true;
		return root;
	}

	cout << "Not all labels are true"<<endl;

	TreeNode root;

	vector<map<double, double>> S_v;

	double totalEntropy = calculateTotalEntropy(l);

	cout << "Total label entropy " << totalEntropy << endl;


	int A = bestAttributeThatClassifiesS(S, attributes, l, totalEntropy);

	cout << A << endl;

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
