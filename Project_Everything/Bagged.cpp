/*
 * ID3.cpp
 *
 *  Created on: Dec 1, 2017
 *      Author: adam
 */

#include "Bagged.h"

Bagged::Bagged() {
	// TODO Auto-generated constructor stub
//	Files f;
//	trainingFiles = f.trainingFiles;
//	testingFiles = f.testingFiles;
//	forest = new vector<TreeNode>;

}

Bagged::~Bagged() {
	// TODO Auto-generated destructor stub
}

bool Bagged::seeIfAllTrue(vector<double> * l) {
	for (int i = 0; i < l->size(); i++) {
		if (l->at(i) == -1) {
//			cout << "returning a flase"<<endl;
			return false;
		}
	}
//	cout << "returning all true"<<endl;
	return true;
}

void Bagged::getAverages(string filepath) {

	for(int i = 0 ; i < 16; i ++){
		discretizedFeatureValues.push_back(0);
	}

	int numExamples = 0;
	ifstream pipein(filepath.c_str());

	for (string line; getline(pipein, line);) {
		numExamples++;

//		double label = ((line.at(0) == '-') ? -1 : 1);
		//              labels.push_back(label);
		line = line.substr(2); //2 was og 1
		std::string token;
		std::istringstream tokenStream(line);
		float key;
		while (std::getline(tokenStream, token, ' ')) {
			key = atof(token.c_str());

			size_t pos = token.find(":");      // position of "live" in str

			string str3 = token.substr(pos + 1);   // get from "live" to the end

			double featValue = stod(str3);

			discretizedFeatureValues.at(key - 1) += featValue;
//
		}

	}

	//now i have the sum of all of the valules. now just divide by line count to get total.
//      cout << "number of examples " << numExamples<< endl;
	for (int i = 0; i < discretizedFeatureValues.size(); i++) {
		discretizedFeatureValues.at(i) /= numExamples;
//              cout << "average feat value for feat #" << i << " is "
//                              << discretizedFeatureValues.at(i) << endl;
	}

}

void Bagged::stream(string filepath, bool isTest) {
	ifstream pipein(filepath.c_str());
	double right = 0, wrong = 0, pos = 0, neg = 0;
	double groupYesVote = 0, groupNoVote = 0;
//	double right = 0, wrong = 0;
	for (string line; getline(pipein, line);) {
		map<double, double> example;
		double label = ((line.at(0) == '0') ? -1 : 1);
		//		labels.push_back(label);
		line = line.substr(2); //2 was og 1
		std::string token;
		std::istringstream tokenStream(line);
		float key;
		while (std::getline(tokenStream, token, ' ')) {
			key = atof(token.c_str());
			//if > average at the key in discretizedfeaturevector then 1
			size_t pos = token.find(":");      // position of "live" in str

			string str3 = token.substr(pos + 1);   // get from "live" to the end

			double featValue = stod(str3);
			if (featValue > discretizedFeatureValues.at(key - 1)) {
				example.insert(make_pair(key, 1));
				featuresMentioned.insert(key);
			}

		}
		if (!isTest) {
			labels.push_back(label);
			if (label != 1 && allLabelsTrue) {
				allLabelsTrue = false;
			}
			trainingMap.push_back(example);
		} else {
			map<double, double> toAddToSVM;
			groupYesVote = 0, groupNoVote = 0;
			for (int tree = 0; tree < forest.size(); tree++) {
				BaggedTreeNode current = forest.at(tree);
//				cout << forest.size() << endl;

				while (current.leaf == false) {
					int attr = current.attributeNum;
//					cout << "Attr num" << attr << endl;
					if (example.find(attr) == example.end()) {
						current = current.branches[0];
					} else {
						current = current.branches[1];
					}
				}

				if (current.label_value == 1) {
					//TODO added a bagged boolean so you dont always do this.

					toAddToSVM.insert(make_pair(current.attributeNum, 1));
					weightsSVM[current.attributeNum] = 0;
					weightsLog[current.attributeNum] = 0;
					//where map used to be
					groupYesVote++;
				} else if (current.label_value == 0) {
					groupNoVote++;
				}

				numpre++;

			}
			mapSVM.push_back(toAddToSVM);
			labelsSVM.push_back(label);

			mapLog.push_back(toAddToSVM);
			labelsLog.push_back(label);
			double guess = 0;
//			cout << "group yes / no " << groupYesVote << " " << groupNoVote
//					<< "\t " ;
			//trying to bias the algorithm
			if (groupYesVote/2 > groupNoVote) {
				guess = 1;
			} else {
				guess = -1;
			}

			predictions.push_back(guess);
//
//			cout << "My guess was " << guess << " real label is " << label
//					<< endl;

			if (guess == label) {
				right++;
			} else {
				wrong++;
			}
//			numpre = 0;
		}
	}
	if (isTest) {

//			percentageCross = right / (right + wrong);
//		cout << "\nGroup decision on the test, yes: " << groupYesVote
//				<< " and the no " << groupNoVote << ", ";
		cout << "\n\tAccuracy " << right / (right + wrong) << endl;

		//		cout << "Right: " << right << " wrong: " << wrong << endl;
	}
}

double Bagged::calculateTotalEntropy(vector<double> * l) {
//	cout << "calculating total entropy " << endl;
	double totalEntropy = 0;
	double posCount = 0;

	for (int label = 0; label < l->size(); label++) {
		if (l->at(label) == 1) {
			posCount++;
		}
	}

	double proportion = posCount / l->size();

	double posPro = proportion;
	double negPro = 1 - (proportion);

	totalEntropy = (-1 * posPro * log2(posPro)) - (negPro * log2(negPro));

	return totalEntropy;
}

double Bagged::calculateEntropy(vector<map<double, double>> * S,
		vector<double> * l, int featureNumber) {
//	cout << "Calculating entropy for feat # " << featureNumber << endl;
	double expectedEntropy = 0;

	double yesEntropy = 0;
	double noEntropy = 0;

	double yesPosCount = 0;
	double yesNegCount = 0;

	double noPosCount = 0;
	double noNegCount = 0;

	for (int row = 0; row < S->size(); row++) {
		map<double, double>::iterator sIter;
		sIter = S->at(row).find(featureNumber);

		if (sIter != S->at(row).end()) {
			//the feature is there and TRUE
			if (l->at(row) == 1) {
				yesPosCount++;
			} else {
				yesNegCount++;
			}
		} else {
			//not there. value if negative.
			if (l->at(row) == 1) {
				noPosCount++;
			} else {
				noNegCount++;
			}
		}
	}

	double yesTotal = yesPosCount + yesNegCount;
//	cout << "yesPos count " << yesPosCount << " yes neg count " << yesNegCount << endl;

	if (yesPosCount != 0 && yesNegCount != 0) {
		yesEntropy = (-1 * (yesPosCount / yesTotal)
				* log2(yesPosCount / yesTotal)
				- (yesNegCount / yesTotal) * log2(yesNegCount / yesTotal));

	} else {
		yesEntropy = 0;
	}

	double noTotal = noPosCount + noNegCount;

	if (noPosCount != 0 && noNegCount != 0) {
		noEntropy = (-1 * (noPosCount / noTotal) * log2(noPosCount / noTotal)
				- (noNegCount / noTotal) * log2(noNegCount / noTotal));
	} else {
		noEntropy = 0;
	}

//	cout << " Yes entropy : " << yesEntropy << endl;
//	cout << " No entropy : " << noEntropy << endl;

	double yesCoef = yesTotal / l->size();
	double noCoef = noTotal / l->size();

//	cout << "Yes coef " << yesCoef << " no coef: " << noCoef <<  " l.size() " << l->size() << endl;

	expectedEntropy = (yesCoef * yesEntropy) + (noCoef * noEntropy);

//	cout <<
//	cout << "Expected entropy " << expectedEntropy << endl;

	return expectedEntropy;
}

double Bagged::calculateGain(vector<map<double, double>> * S, vector<double> * l,
		double totalEntropy, int featureNumber) {

//	double calculateEntropy(vector<map<double,double>> S, vector<double> l, int featureNumber);

//	if (zeroData.find(featureNumber) == zeroData.end()) {
//		return totalEntropy;
//	}
	double expectedEntropy = calculateEntropy(S, l, featureNumber);
//	if(featureNumber == 0)
//	cout << "expected entropy " << expectedEntropy << " for feat # " << featureNumber << endl;
	return totalEntropy - expectedEntropy;
}

int Bagged::bestAttributeThatClassifiesS(vector<map<double, double>> * S,
		set<int> * attributes, vector<double> * l, double totalEntropy) {

	double maxGain = 0;
	double maxFeat = 0;

	int count = 0;

	//calculate gain for every feature.
	for (set<int>::iterator attIter = attributes->begin();
			attIter != attributes->end(); attIter++) {

		if (attIter == attributes->begin()) {
			attIter++;
		}

		double gain = calculateGain(S, l, totalEntropy, *attIter);

//		if (gain == 0) {
//			zeroData.insert(maxFeat);
//		}

		if (gain > maxGain) {
//			cout << "New max gain " << gain << " @ " << *attIter << endl;
			maxGain = gain;
			maxFeat = *attIter;
		}

		if (count >= 325) {
			return maxFeat;
		}

		count++;
	}
	return maxFeat;
}

BaggedTreeNode Bagged::recurse(vector<map<double, double>> * S, set<int> * attributes,
		vector<double> * l, int currentDepth) {

//	currentDepth++;
//	cout << "S.size " << S->size() << " l.size() " << l->size() << endl;

	if (seeIfAllTrue(l)) {
		BaggedTreeNode root;
		root.leaf = true;
		root.label_value = true;
		return root;
	}

	BaggedTreeNode root;

	vector<map<double, double>> S_v;

	double totalEntropy = calculateTotalEntropy(l);

//	cout << "Total label entropy " << totalEntropy << endl;

	int A = bestAttributeThatClassifiesS(S, attributes, l, totalEntropy);

//	cout << "The best attirubte is  " << A << endl;

	/**
	 *
	 *
	 *
	 *
	 *
	 */

//	attributes->erase(A);
	root.attributeNum = A;
	root.leaf = false;

	for (int i = 0; i < 2; i++) {
		BaggedTreeNode child;
		child.attribute = A;
		vector<map<double, double> > sv;
		vector<double> labels_sv;
		for (int j = 0; j < S->size(); j++) {
			if (i == 0) {
				if (S->at(j).find(A) == S->at(j).end()) {
					sv.push_back(S->at(j));
					labels_sv.push_back(l->at(j));
				}
			} else {
				if (S->at(j).find(A) != S->at(j).end()) {
					sv.push_back(S->at(j));
					labels_sv.push_back(l->at(j));
				}
			}
		}

		if (sv.size() == 0 || currentDepth >= maxDepth) {
			child.leaf = true;

			int pC = 0;
			int nC = 0;
			for (int z = 0; z < labels_sv.size(); z++) {
//				cout << "l : " << labels_sv.at(z) << endl;
				if (labels_sv.at(z) == 1) {
					pC++;
				} else {
					nC++;
				}
			}
			bool temp;
			if (nC > pC) {
				temp = false;
			} else {
				temp = true;
			}
			child.label_value = temp;
//			cout << "yes " << pC << " no " << nC << endl;
//			cout << "setting child with label " << child.label_value << endl;
			currentDepth++;
			root.branches.push_back(child);
			//return new_node;
		} else {
//			cout << "Pushing back new node " << endl;
			root.branches.push_back(
					recurse(&sv, attributes, &labels_sv, ++currentDepth));

		}
	}

	return root;

}

void Bagged::test(string filepath) {

	stream(filepath, true);

	//just finished bag forests.
	//now running SVM over bagged
//	cout << "***\n***\nBeginning SVM over bagged forest" << endl;

}

void Bagged::run(vector<map<double, double>> * S, vector<double> * l,
		set<int> * attributes) {
	forest.push_back(recurse(S, attributes, l, 0));
}

void Bagged::bagged(bool isSVM) {

//	cout << "Beginning bagged forests on training set" << endl;

//	if(isSVM){
//		cout << "Will be running SVM over bagged forest" << endl;
//	}

//	cout << "test"<<endl;
	getAverages(trainingFiles.at(0));

//	for(int i = 0; i < 16; i++){
//		cout << "Average value for feature #" << i << " is " << discretizedFeatureValues.at(i) << endl;
//	}

	stream(trainingFiles.at(0), false); //training map consists of the entire file now with positives.
//	stream(trainingFiles.at(1), false); //training map consists of the entire file now with positives.
//	stream(trainingFiles.at(2), false); //training map consists of the entire file now with positives.
	//now split up the data 100 ways

	/* initialize random seed: */
	srand(50);

	for (int treeNumber = 0; treeNumber < 1000; treeNumber++) {
		vector<map<double, double>> S;
		vector<double> l;
		set<int> attributes = featuresMentioned;

//		cout << "Tree num " << treeNumber << endl;
		for (int example = 0; example < 100; example++) {
			int randomRow = rand() % trainingMap.size();
//			cout << "random row " << randomRow << endl;
			S.push_back(trainingMap.at(randomRow));
			l.push_back(labels.at(randomRow));
		}

		run(&S, &l, &attributes);

		S.clear();
		l.clear();

	}

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

//    cout << "Id,Prediction" << endl;

    for (int i = 0; i < predictions.size(); i++) {
            int guess = (predictions.at(i) == -1) ? 0 : 1;
//            cout << userIDs.at(i) << "," << guess << endl;
    }

	trainingMap.clear();
	labels.clear();
	featuresMentioned.clear();
	zeroData.clear();


}

void Bagged::go() {
//	stream("data/speeches.train.liblinear", false); //training map consists of the entire file now with positives.

//	for (int i = 0; i < 100; i++) {


//		cout << i << endl;
//	test("data/speeches.test.liblinear");

//	}

}
