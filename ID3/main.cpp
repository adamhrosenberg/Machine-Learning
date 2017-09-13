#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <string>
#include <string.h>
#include <math.h>
#include <set>
#include <map>

using namespace std;

int NUM_FEATURES = 8;
int BINARY = 2;
//set<int> reductionSet;
typedef struct TreeNode {
public:
	string attribute;
	int attributeNum;
	vector<TreeNode> branches;
	bool leaf;
	bool label_value;
} TreeNode;

class MatrixWrapper {

public:

	MatrixWrapper(string file, vector<string> * names, vector<bool> * labels,
			vector<vector<bool> > * atrributetable);
	void populateAttributeTable(vector<string> names,
			vector<vector<bool> >* attributetable);

	void printAttributeTable(vector<vector<bool> >* atrributetable);

//	void computeLabelEntropy(vector<bool> labels);

//	float computeFeatureGain(vector<bool> labels,
//	vector<vector<bool> > atrributetable, int featureNumber);

//	float computeOverallME(vector<bool> feature);

	/*
	 *   	0. Is their first name longer than their last name?
	 */
	bool rule0(vector<string> name) {
		int first_length = name.at(0).size();
		int last_length = name.at(name.size() - 1).size();
		return first_length > last_length;
	}
	/*
	 * 1. Do they have a middle name?
	 */
	bool rule1(vector<string> name) {
		return name.size() > 2;
	}

	/*
	 * 2. Does their first name start and end with the same letter? (ie "Ada")
	 */
	bool rule2(vector<string> name) {
		int first_name_length = name.at(0).size();
		char first_letter = name.at(0).at(0);
		char last_letter = name.at(0).at(first_name_length - 1);
//		cout << first_letter << " vs " << last_letter << endl;
		return first_letter == last_letter;
	}

	/*
	 * 3. Does their first name come alphabetically before their last name? (ie "Dan Klein" because "d" comes before "k")
	 *
	 */
	bool rule3(vector<string> name) {

		string first_name = name.at(0);
		string last_name = name.at(name.size() - 1);
//		cout << "NAME: " << first_name << " "<< last_name;
		int k;
		k = first_name.compare(last_name);
//		cout << " " << k <<endl;
		if (k < 0)
			return true;
		else
			return true;

	}

	/*
	 * 4. Is the second letter of their first name a vowel (a,e,i,o,u)?
	 */
	bool rule4(vector<string> name) {
		if (name.at(0).size() == 1)
			return false;
		return name.at(0).at(1) == 'a' || name.at(0).at(1) == 'e'
				|| name.at(0).at(1) == 'i' || name.at(0).at(1) == 'o'
				|| name.at(0).at(1) == 'u';
	}

	/*
	 * 5. Is the number of letters in their last name even?
	 */
	bool rule5(vector<string> name) {
		return name.at(name.size() - 1).size() % 2 == 0;
	}

	//first and last name same letter
	bool rule6(vector<string> name) {
		char firstLetter = name.at(0).at(0);
		char lastLetter = name.at(name.size() - 1).at(0);

		return firstLetter == lastLetter;
	}

	//
	bool rule7(vector<string> name) {
		return name.at(0).size() > 5;
	}
}
;

/*
 * Constructor for Matrix class. Takes in the file
 * to feed into the vector<vector<string>> Matrix.
 */
MatrixWrapper::MatrixWrapper(string file, vector<string> * names,
		vector<bool> * labels, vector<vector<bool> > * atrributetable) {
	int linenum = 0;
	//feed in text file here
	ifstream pipein(file.c_str());

	//for each line in the file..
	for (string line; getline(pipein, line);) {
		linenum++;
		if (line.at(0) == '+') {
			labels->push_back(true);
		} else {
			labels->push_back(false);
		}

		//push in the names.
		line = line.substr(2, line.size() - 1);
		::transform(line.begin(), line.end(), line.begin(), ::tolower);
		names->push_back(line);
		//			cout << line << endl;

	}

	//instantiate att table
	vector<bool> row(NUM_FEATURES, false);
	for (int i = 0; i < names->size(); i++) {
		atrributetable->push_back(row);
	}
}

void MatrixWrapper::populateAttributeTable(vector<string> names,
		vector<vector<bool> >* attributetable) {
	for (int i = 0; i < names.size(); i++) {

		string str(names.at(i));
		string buf;
		stringstream ss(str);
		vector<string> tokens;

		//tokenize name
		while (ss >> buf) {
			tokens.push_back(buf);
		}

		//calculate the attribute table
		for (int j = 0; j < NUM_FEATURES; j++) {
			switch (j) {
			case 0:
				attributetable->at(i).at(j) = rule0(tokens);
				break;
			case 1:
				attributetable->at(i).at(j) = rule1(tokens);
				break;
			case 2:
				attributetable->at(i).at(j) = rule2(tokens);
				break;
			case 3:
				attributetable->at(i).at(j) = rule3(tokens);
				break;
			case 4:
				attributetable->at(i).at(j) = rule4(tokens);
				break;
			case 5:
				attributetable->at(i).at(j) = rule5(tokens);
				break;
			case 6:
				attributetable->at(i).at(j) = rule6(tokens);
				break;
			case 7:
				attributetable->at(i).at(j) = rule7(tokens);
			}
		}
	}
}

void MatrixWrapper::printAttributeTable(
		vector<vector<bool> > * atrributetable) {
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < NUM_FEATURES; j++) {
			if (j == 0)
//				cout << names.at(i) << ": \t\t";
				cout << atrributetable->at(i).at(j);
		}
		cout << endl;
	}
}

float computeLabelEntropy(vector<vector<bool> > atrributetable,
		vector<bool> labels, int featureNumber, bool value, bool fullTable) {
	float label_entropy = 0;
	//Entropy(S) = H(S) = -p_+log_2(p_+)-p_-log_2(p_-)
	float positivep = 0, negativep = 0;

	//calculating H(S)
	if (fullTable) {
		for (int i = 0; i < labels.size(); i++) {
			if (labels.at(i)) {
				positivep++;
			} else {
				negativep++;
			}
		}

		if (positivep != 0 && negativep != 0) {
			double posP = positivep / (float) labels.size();
			double negP = negativep / (float) labels.size();
			label_entropy = -posP * log2f(posP) - negP * log2f(negP);
		} else {
//			cout << "Returning 0" << endl;
			return 0;
		}

		//cout << "Overall H(S) = " << label_entropy << endl;
		return label_entropy;
	}

	int countV = 0;
	//not calculating full table.
	for (int i = 0; i < atrributetable.size(); i++) {

		if (atrributetable.at(i).at(featureNumber) == value) {
			countV++;
			if (labels.at(i)) {
				positivep++;
			} else {
				negativep++;
			}

		}
	}

	if (positivep != 0 && negativep != 0) {
		positivep = positivep / (float) labels.size();
		negativep = negativep / (float) labels.size();
		label_entropy = -positivep * log2f(positivep)
				- negativep * log2f(negativep);
	} else {
		return 0;
	}

//	cout << "H(S) = " << label_entropy << endl;
	return label_entropy;
}

float computeFeatureGain(vector<bool> labels,
		vector<vector<bool> > atrributetable, int featureNumber) {

	//Gain(S, Feature) = H(S) - summation |S_v| / |S| H(S_v)
	//for i < attributetable.size()
	vector<bool> trues;
	vector<bool> falses;

	vector<vector<bool> > trueAtts;
	vector<vector<bool> > falseAtts;

	double entropy_s = computeLabelEntropy(atrributetable, labels,
			featureNumber, false, true);

	for (int i = 0; i < atrributetable.size(); i++) {
		if (atrributetable.at(i).at(featureNumber)) {
			trueAtts.push_back(atrributetable.at(i));
			trues.push_back(labels.at(i));
		} else {
			falseAtts.push_back(atrributetable.at(i));
			falses.push_back(labels.at(i));
		}
	}

	double yesH = computeLabelEntropy(trueAtts, trues, featureNumber, true,
			false);
	double noH = computeLabelEntropy(falseAtts, falses, featureNumber, false,
			false);

	yesH *= (((float) trues.size()) / ((float) labels.size()));
	noH *= (((float) falses.size()) / ((float) labels.size()));

	return entropy_s - (yesH + noH);

//
//	gain = computeLabelEntropy(atrributetable, labels, NUM_FEATURES - 1, true,
//			true)
//			- ((yesFraction
//					* computeLabelEntropy(yesSet, labels, featureNumber, true,
//							false)
//					+ (noFraction
//							* computeLabelEntropy(noSet, labels, featureNumber,
//									false, false))));
//	cout << "Gain(Feature_" << featureNumber << "):" << gain << endl;
//	return gain;

}

float computeOverallME(vector<bool> feature) {
	float positivep, negativep, label_majority_error;
	for (int i = 0; i < feature.size(); i++) {
		if (feature.at(i)) {
			positivep++;
		} else {
			negativep++;
		}
	}

	if (positivep > negativep) {
		label_majority_error = 1 - (positivep / feature.size());
	} else {
		label_majority_error = 1 - (negativep / feature.size());
	}

	//	cout << "Label ME = " << label_majority_error << endl;
	return label_majority_error;
}

void printTree(TreeNode* root, int space) {
	if (root->branches.size() == 0) {
		cout << root->attribute << endl;
		return;
	}
	space += 10;
	printTree(&root->branches.at(0), space);
	cout << endl;
	for (int i = 0; i < space; i++) {
		cout << " ";
	}
	cout << root->attribute << endl;
}
void printTreeDriver(TreeNode* root) {
	printTree(root, 0);
}

TreeNode ID3(vector<vector<bool> > atrributetable, vector<string> names,
		vector<bool> labels, int maxDepth, int currentDepth,
		set<int> reductionSet) {

	TreeNode currentNode;
	currentNode.leaf = false;
	currentNode.label_value = false;

	if (currentDepth == maxDepth) {
		int posCount = 0, negCount = 0;
		for (int i = 0; i < names.size(); i++) {
			if (labels.at(i)) {
				posCount++;
			} else {
				negCount++;
			}
		}
		if (posCount > negCount) {
			currentNode.label_value = true;
		} else {
			currentNode.label_value = false;
		}
		currentNode.leaf = true;
//		cout << "reached max depth, returning leaf, depth = " << currentDepth << endl;
		return currentNode;
	}
//	MatrixWrapper matrix;
	//if all examples have same label: return a single node tree w/ the label.
	bool sameFlag = true;
	bool firstLabel = labels.at(0);
	for (int i = 0; i < labels.size(); i++) {
		if (!(labels.at(i) && (firstLabel))) {
			sameFlag = false;
			//i = labels.size();
		}
	}

	if (sameFlag) {
		currentNode.leaf = true;
		currentNode.attribute = "all";
		currentNode.attributeNum = 0;
		currentNode.label_value = labels.at(0);
		return currentNode;
	}

//	cout << "Size of reduction set " << reductionSet.size() << endl;
	if (reductionSet.size() >= NUM_FEATURES) {
		cout << "reduction set is full" << endl;

		int posCount = 0, negCount = 0;
		for (int i = 0; i < names.size(); i++) {
			if (labels.at(i)) {
				posCount++;
			} else {
				negCount++;
			}

		}
		if (posCount > negCount) {
			currentNode.label_value = true;
		} else {
			currentNode.label_value = false;
		}
		//todo set att num.
		currentNode.leaf = true;
		cout << "************Returning leaf node w/ full reduction set."
				<< endl;
		currentNode.attributeNum = 0;
		return currentNode;
	}
	//computeLabelEntropy(atrributetable, NUM_FEATURES - 1, true);
	//computeOverallME(labels);

	int maxGainFeatureIndex = 0;
	float maxGain = 0;

	for (int i = 0; i < atrributetable.at(0).size(); i++) {
		if (!reductionSet.count(i)) {
			float temp = computeFeatureGain(labels, atrributetable, i);
			if (temp >= maxGain) {
				maxGain = temp;
				maxGainFeatureIndex = i;
			}
			//cout << endl;
		}
	}
//	cout << "Root node is feature # : " << maxGainFeatureIndex << endl;

	//we have a root node ladies and gentleman
	currentNode.attributeNum = maxGainFeatureIndex;
	set<int>::iterator it;
	reductionSet.insert(maxGainFeatureIndex);

	//size
//		int count = 0; //magnitude of new set.
	//create children branches
	for (int i = 0; i < BINARY; i++) {
		vector<vector<bool> > tempAttTable;
		vector<string> tempNames;
		vector<bool> tempLabels;
//		cout << "creating child branches " << i << endl;
		TreeNode child;
		for (int j = 0; j < atrributetable.size(); j++) {
			bool flag;
			if (i == 0)
				flag = false;
			else if (i == 1)
				flag = true;

			if (atrributetable.at(j).at(maxGainFeatureIndex) == flag) {
				//names.erase(names.begin() + j);
				//labels.erase(labels.begin() + j);
				//removeRowFromTable(j);
//					count++;
				tempAttTable.push_back(atrributetable.at(j));
				tempNames.push_back(names.at(j));
				tempLabels.push_back(labels.at(j));
			}
		}

		//now we need to remove that attribute column from the attribute table.
		//if we need to set a pure label / leaf node
		if (tempNames.size() == 0) {
			float positivep = 0, negativep = 0; //set these to = 0
			for (int k = 0; k < names.size(); k++) {
				if (labels.at(k)) {
					positivep++;
				} else {
					negativep++;
				}
			}

//			cout << "returning back child LEAF w/ label value = ";
			if (positivep > negativep) {
				child.label_value = true;
//				cout << " true, current depth = " << currentDepth << endl;
			} else {
				child.label_value = false;
//				cout << " false , current depth = " << currentDepth << endl;
			}
			child.leaf = true;
//			return child; //currentNode
//			currentNode.branches.push_back(child);
			return child;
		} else {

			currentDepth++;
			currentNode.leaf = false;
//			cout << "\n***Recursing***" << " branch #: "
//					<< currentNode.branches.size() << " , depth = " << currentDepth<< endl;
			currentNode.branches.push_back(
					ID3(tempAttTable, tempNames, tempLabels, maxDepth,
							currentDepth, reductionSet));
		}

	}
	return currentNode;
}

TreeNode createTreeDriver(vector<vector<bool> > atrributetable,
		vector<string> names, vector<bool> labels, int maxDepth,
		int currentDepth) {

	set<int> reductionSet;
	TreeNode root = ID3(atrributetable, names, labels, maxDepth, currentDepth,
			reductionSet);

	return root;

}

bool goDownPath(TreeNode node, vector<bool> attRow) {
	if (node.leaf)
		return node.label_value;

	bool action = attRow.at(node.attributeNum);
	bool answer = false;

	if (action)
		answer = goDownPath(node.branches.at(0), attRow);
	else
		answer = goDownPath(node.branches.at(1), attRow);
	return answer;
}

int main(int argc, char* argv[]) {

	vector<string> fileNames;
	fileNames.push_back(
			"Updated_Dataset/Updated_CVSplits/updated_training00.txt");
	fileNames.push_back(
			"Updated_Dataset/Updated_CVSplits/updated_training01.txt");
	fileNames.push_back(
			"Updated_Dataset/Updated_CVSplits/updated_training02.txt");
	fileNames.push_back(
			"Updated_Dataset/Updated_CVSplits/updated_training03.txt");

	float numRight = 0, numWrong = 0;


	for (int index = 0; index < fileNames.size(); index++) {
		cout << endl;
		for (int innerIndex = 1; innerIndex < NUM_FEATURES; innerIndex++) {
			vector<vector<bool> > atrributetable;
			vector<bool> labels;
			vector<string> names;
			MatrixWrapper matrix(fileNames.at(index), &names, &labels,
					&atrributetable);

			matrix.populateAttributeTable(names, &atrributetable);

			TreeNode root;
			root = createTreeDriver(atrributetable, names, labels, innerIndex, 0);

			vector<bool> testAnswers;

			for (int i = 0; i < names.size(); i++) {
				bool action = goDownPath(root, atrributetable.at(i));
				testAnswers.push_back(action);
			}

			for (int i = 0; i < testAnswers.size(); i++) {
				if (testAnswers.at(i) == labels.at(i)) {
					numRight++;
				} else {
					numWrong++;
				}
			}

			float percent = numRight / (numRight + numWrong);

			cout << "File #" << fileNames.at(index).substr(50) << " accuracy =  " << percent << "%, with a depth of: "
					<< innerIndex << endl;
		}

	}

	return 0;
}
