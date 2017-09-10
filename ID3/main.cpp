#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <string>
#include <math.h>
#include <map>

using namespace std;

int NUM_FEATURES = 6;
vector<vector<bool> > atrributetable;
vector<bool> labels;
float label_entropy = 0;
float label_majority_error = 0;
vector<string> names;

typedef struct TreeNode {
public:
	string attribute;
	int attributeNum;
	vector<TreeNode *> branches;
	bool leaf;
	bool label_value;
} TreeNode;

class MatrixWrapper {

public:

	MatrixWrapper(char* file);
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
	 */
	bool rule3(vector<string> name) {
		string first_name = name.at(0);
		string last_name = name.at(name.size() - 1);
		//int k = 0;
		// k = strcmp(first_name.c_str(), last_name.c_str());
//		int k = strcmp(first_name.c_str(), last_name.c_str());
		return false;
	}

	/*
	 * 4. Is the second letter of their first name a vowel (a,e,i,o,u)?
	 */
	bool rule4(vector<string> name) {
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

}
;

/*
 * Constructor for Matrix class. Takes in the file
 * to feed into the vector<vector<string>> Matrix.
 */
MatrixWrapper::MatrixWrapper(char* file) {
	int linenum = 0;
	//feed in text file here
	ifstream pipein(file);

	//for each line in the file..
	for (string line; getline(pipein, line);) {
		linenum++;
		if (line.at(0) == '+') {
			labels.push_back(true);
		} else {
			labels.push_back(false);
		}

		//push in the names.
		line = line.substr(2, line.size() - 1);
		::transform(line.begin(), line.end(), line.begin(), ::tolower);
		names.push_back(line);
		//			cout << line << endl;

	}

	//instantiate att table
	vector<bool> row(NUM_FEATURES, false);
	for (int i = 0; i < names.size(); i++) {
		atrributetable.push_back(row);
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
			}
		}
	}
}

void MatrixWrapper::printAttributeTable(vector<vector<bool> >* atrributetable) {
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < NUM_FEATURES; j++) {
			if (j == 0)
				cout << names.at(i) << ": \t\t";
			cout << atrributetable->at(i).at(j);
		}
		cout << endl;
	}
}

void computeLabelEntropy(vector<bool> labels) {
	//Entropy(S) = H(S) = -p_+log_2(p_+)-p_-log_2(p_-)
	float positivep, negativep;
	for (int i = 0; i < labels.size(); i++) {
		if (labels.at(i)) {
			positivep++;
		} else {
			negativep++;
		}
	}
//		cout << "# of positive = " << positivep << endl;
//		cout << "# of negative = " << negativep << endl;
//		cout << "# of total = " << labels.size() << endl;
	if (positivep != 0 && negativep != 0) {
		positivep = positivep / labels.size();
		negativep = negativep / labels.size();
		label_entropy = -positivep * log2f(positivep)
				- negativep * log2f(negativep);
	} else {
		label_entropy = 0;
	}

	cout << "H(S) = " << label_entropy << endl;
}

float computeFeatureGain(vector<bool> labels,
		vector<vector<bool> > atrributetable, int featureNumber) {

	//Gain(S, Feature) = H(S) - summation |S_v| / |S| H(S_v)
	//for i < attributetable.size()

	float numberOfYes, numberOfNo;
	float positivep = 0, negativep = 0;
	float gain, expectedYesH, expectedNoH;

	//first calculate yes/no expected entropy.
	for (int i = 0; i < atrributetable.size(); i++) {
		if (atrributetable.at(i).at(featureNumber)) {
			numberOfYes++; //number of YES in column
			if (labels.at(i)) {
				positivep++; //number of YES with label YES.
			}

		} else {
			numberOfNo++;
			if (labels.at(i)) {
				negativep++; //number of NO with label YES
			}
		}
	}

	float total = numberOfYes + numberOfNo;
	float noFraction = numberOfNo / total;
	float yesFraction = numberOfYes / total;
	float secondterm, yesTerm, noTerm;

	if (numberOfYes == 0 || numberOfNo == 0) {
		return 0;
	}
	if (positivep != numberOfYes && positivep != 0 && numberOfYes != 0) {
		secondterm = (numberOfYes - positivep) / numberOfYes;
		yesTerm = positivep / numberOfYes;
		//compute for yes
		if (yesTerm != 0 && secondterm != 0) {
			expectedYesH = -yesTerm * log2f(yesTerm)
					- secondterm * log2f(secondterm);
//							cout << "H(S) yes = " << expectedYesH << endl;
		} else {
			expectedYesH = 0;
//				cout << "H(S) yes = " << expectedYesH <<endl;
		}

	}

	//now compute for no
	secondterm = (numberOfNo - negativep) / numberOfNo;
	noTerm = negativep / numberOfNo;

	expectedNoH = -noTerm * log2f(noTerm) - secondterm * log2f(secondterm);
//		cout << "H(S no) = " << expectedNoH << endl;

	gain = label_entropy
			- ((yesFraction * expectedYesH) + (noFraction * expectedNoH));
	cout << "Gain(Feature_" << featureNumber << "):" << gain << endl;
	return gain;

}

float computeOverallME(vector<bool> feature) {
	float positivep, negativep;
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

void removeColumnFromTable(int maxGainFeatureIndex) {

	for(int i = 0; i < atrributetable.size(); i++){
		if(atrributetable.at(i).size() > maxGainFeatureIndex){
			atrributetable.at(i).erase(atrributetable.at(i).begin() + maxGainFeatureIndex);
		}
	}
	//decrease feature count
	NUM_FEATURES --;

	//print out the table to check.
//	for (int i = 0; i < atrributetable.size(); i++) {
//		for (int j = 0; j < NUM_FEATURES; j++) {
//			if (j == 0)
//				cout << names.at(i) << ": \t\t";
//			cout << atrributetable.at(i).at(j);
//		}
//		cout << endl;
//	}

}

TreeNode ID3() {
//	MatrixWrapper matrix;
	//if all examples have same label: return a single node tree w/ the label.
	bool sameFlag = true;
	bool firstLabel = labels.at(0);
	for (int i = 0; i < labels.size(); i++) {
		if (!(labels.at(i) && (firstLabel))) {
			sameFlag = false;
			i = labels.size();
		}
	}

	TreeNode currentNode;
	if (sameFlag) {
		currentNode.leaf = true;
		currentNode.attribute = "all";
		return currentNode;
	} else {
		//calculate information gain by using matrix wrapper methods.

		currentNode.leaf = true;

		//A = attribute in Attributes that BEST classifies S.
		//find majority (betwen true and false) and set that to node.label_value
		int numTrue = 0, numFalse = 0;
		for (int i = 0; i < names.size(); i++) {
			if (labels.at(i)) {
				numTrue++;
			} else {
				numFalse++;
			}
		}
		//currentNode.label_value = (numTrue > numFalse);
		//for each possible value v of that A can take
		//add a new tree branch corresponding to A=v
		//Let s_v be the subset of examples in S w/ A=v
		//if s_v is empty:
		//add lead node w/ common value of Label in S
		//ID3(S_v, Attributes - {A}, Label)

		computeLabelEntropy(labels);
		computeOverallME(labels);

		int maxGainFeatureIndex = 0;
		float maxGain = 0;
		for (int i = 0; i < NUM_FEATURES; i++) {
			float temp = computeFeatureGain(labels, atrributetable, i);
			if (temp > maxGain) {
				maxGain = temp;
				maxGainFeatureIndex = i;
			}
		}
		cout << "Root node is feature # : " << maxGainFeatureIndex << endl;

		//we have a root node ladies and gentleman
		currentNode.attributeNum = maxGainFeatureIndex;
		//now we need to remove that attribute column from the attribute table.
		removeColumnFromTable(maxGainFeatureIndex);

	}

}

TreeNode createTreeDriver() {

	TreeNode root = ID3();
	return root;
}

void printTree(TreeNode* root, int space) {
	if (root->branches.size() == 0) {
		cout << root->attribute << endl;
		return;
	}
	space += 10;
	printTree(root->branches.at(0), space);
	cout << endl;
	for (int i = 0; i < space; i++) {
		cout << " ";
	}
	cout << root->attribute << endl;
}
void printTreeDriver(TreeNode* root) {
	printTree(root, 0);
}

int main(int argc, char* argv[]) {

	//	MatrixWrapper Matrix("Updated_Dataset/updated_train.txt");
	MatrixWrapper matrix(
			"Updated_Dataset/Updated_CVSplits/updated_training00.txt");
	matrix.populateAttributeTable(names, &atrributetable);
	//Matrix.printAttributeTable(&Matrix.atrributetable);

	TreeNode root;
	root = createTreeDriver();
	printTreeDriver(&root);
	return 0;
}
