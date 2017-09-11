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
#include <set>
#include <map>

using namespace std;

int NUM_FEATURES = 6;
int BINARY = 2;

int treeDepth = 0;
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

	MatrixWrapper(char* file, vector<string> * names, vector<bool> * labels, vector<vector<bool> > * atrributetable);
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
MatrixWrapper::MatrixWrapper(char* file, vector<string> * names, vector<bool> * labels, vector<vector<bool> > * atrributetable) {
	int linenum = 0;
	//feed in text file here
	ifstream pipein(file);

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
			}
		}
	}
}

void MatrixWrapper::printAttributeTable(vector<vector<bool> > * atrributetable) {
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
			positivep = positivep / (float)labels.size();
			negativep = negativep / (float)labels.size();
			label_entropy = -positivep * log2f(positivep)
					- negativep * log2f(negativep);
		} else {
			label_entropy = 0;
		}

		cout << "H(S) = " << label_entropy << endl;
		return label_entropy;
	}

	//not calculating full table.
	for (int i = 0; i < atrributetable.size(); i++) {
		if (atrributetable.at(i).at(featureNumber) == value) {
			positivep++;
		} else {
			negativep++;
		}
	}

	if (positivep != 0 && negativep != 0) {
		positivep = positivep / (float)labels.size();
		negativep = negativep / (float)labels.size();
		label_entropy = -positivep * log2f(positivep)
				- negativep * log2f(negativep);
	} else {
		label_entropy = 0;
	}

	cout << "H(S) = " << label_entropy << endl;
	return label_entropy;
}

float computeFeatureGain(vector<bool> labels,
		vector<vector<bool> > atrributetable, int featureNumber) {

	//Gain(S, Feature) = H(S) - summation |S_v| / |S| H(S_v)
	//for i < attributetable.size()

	float numberOfYes, numberOfNo;
	float positivep = 0, negativep = 0;
	float gain, expectedYesH, expectedNoH;
	float total = numberOfYes + numberOfNo;
	float noFraction = numberOfNo / total;
	float yesFraction = numberOfYes / total;
	float secondterm, yesTerm, noTerm;
	vector<vector<bool> > yesSet;
	vector<vector<bool> > noSet;

	//first calculate yes/no expected entropy.
	for (int i = 0; i < atrributetable.size(); i++) {
		if (atrributetable.at(i).at(featureNumber)) {
			numberOfYes++; //number of YES in column
			yesSet.push_back(atrributetable.at(i));
			if (labels.at(i)) {
				positivep++; //number of YES with label YES.
			}
		} else {
			numberOfNo++;
			noSet.push_back(atrributetable.at(i));
			if (labels.at(i)) {
				negativep++; //number of NO with label YES
			}
		}
	}

	gain = computeLabelEntropy(atrributetable, labels, NUM_FEATURES - 1, true,
			true)
			- ((yesFraction
					* computeLabelEntropy(yesSet, labels, featureNumber, true,
							false)
					+ (noFraction
							* computeLabelEntropy(noSet, labels, featureNumber,
									false, false))));
	cout << "Gain(Feature_" << featureNumber << "):" << gain << endl;
	return gain;

}

/*float computeOverallME(vector<bool> feature) {
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
}*/

void removeColumnFromTable(int maxGainFeatureIndex) {

//	for (int i = 0; i < atrributetable.size(); i++) {
//		if (atrributetable.at(i).size() > maxGainFeatureIndex
//				&& atrributetable.at(i).size() > 1) {
//			atrributetable.at(i).erase(
//					atrributetable.at(i).begin() + maxGainFeatureIndex);
//		}
//	}
	//decrease feature count
	//NUM_FEATURES--;

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

void removeRowFromTable(int rowToDelete) {
//	if (atrributetable.size() > rowToDelete) {
//		atrributetable.erase(atrributetable.begin() + rowToDelete);
//	}
}

TreeNode ID3(vector<vector<bool> > atrributetable, vector<string> names,
		vector<bool> labels, set<int> reductionSet) {
//	MatrixWrapper matrix;
	//if all examples have same label: return a single node tree w/ the label.
	bool sameFlag = true;
	bool firstLabel = labels.at(0);
	TreeNode currentNode;
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
		return currentNode;
	} else {

		if (reductionSet.size() >= NUM_FEATURES) {
			currentNode.leaf = true;
			int posCount = 0, negCount = 0;
			for (int i = 0; i < names.size(); i++) {
				if (labels.at(i)) {
					posCount++;
				} else {
					negCount++;
				}
				if (posCount >= negCount) {
					currentNode.label_value = true;
				} else {
					currentNode.label_value = false;
				}
			}
			return currentNode;

		}
		//computeLabelEntropy(atrributetable, NUM_FEATURES - 1, true);
		//computeOverallME(labels);

		int maxGainFeatureIndex = 0;
		float maxGain = 0;
		for (int i = 0; i < NUM_FEATURES; i++) {
			if (!reductionSet.count(i)) {
				float temp = computeFeatureGain(labels, atrributetable, i);
				if (temp >= maxGain) {
					maxGain = temp;
					maxGainFeatureIndex = i;
				}
			}
		}
		cout << "Root node is feature # : " << maxGainFeatureIndex << endl;

		//we have a root node ladies and gentleman
		currentNode.attributeNum = maxGainFeatureIndex;

		vector<vector<bool> > tempAttTable;
		vector<string> tempNames;
		vector<bool> tempLabels;

		//size
//		int count = 0; //magnitude of new set.
		//create children branches
		for (int i = 0; i < BINARY; i++) {
			cout << "creating child branches " << i << endl;
			TreeNode child;
			for (int j = 0; j < atrributetable.size(); j++) {
				bool flag;
				if (i == 0)
					flag = false;
				else if (i == 1)
					flag = true;

				if (atrributetable.at(j).at(maxGainFeatureIndex)) {
					//names.erase(names.begin() + j);
					//labels.erase(labels.begin() + j);
					//removeRowFromTable(j);
//					count++;
					tempAttTable.push_back(atrributetable.at(j));
					tempNames.push_back(names.at(j));
					tempLabels.push_back(labels.at(j));
				}
			}
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
			reductionSet.insert(maxGainFeatureIndex);

			//now we need to remove that attribute column from the attribute table.

			//if we need to set a pure label / leaf node
			if (tempNames.size() == 0) {
				float positivep = 0, negativep = 0; //set these to = 0
				for (int i = 0; i < names.size(); i++) {
					if (labels.at(i)) {
						positivep++;
					} else {
						negativep++;
					}
				}

				cout << "returning back child w/ label value = ";
				if (positivep > negativep) {
					child.label_value = true;
					cout << " true " << endl;
				} else {
					child.label_value = false;
					cout << " false " << endl;
				}
				child.leaf = true;
				return child;
//				currentNode.branches.push_back(child);
			} else {
				cout << "\n***Recursing***" << " branch size: "
						<< currentNode.branches.size() << endl;
				treeDepth++;
				currentNode.leaf = false;
				currentNode.branches.push_back(
						ID3(tempAttTable, tempNames, tempLabels, reductionSet));
			}
		}

	}
	return currentNode;
}

TreeNode createTreeDriver(vector<vector<bool> > atrributetable, vector<string> names, vector<bool> labels) {

	set<int> reductionSet;
	TreeNode root = ID3(atrributetable, names, labels, reductionSet);
	return root;
}

int main(int argc, char* argv[]) {

	//	MatrixWrapper Matrix("Updated_Dataset/updated_train.txt");
	vector<vector<bool> > atrributetable;
	vector<bool> labels;
	vector<string> names;

	MatrixWrapper matrix(
			"Updated_Dataset/Updated_CVSplits/updated_training00.txt",&names, &labels, &atrributetable);
	matrix.populateAttributeTable(names, &atrributetable);
	//Matrix.printAttributeTable(&Matrix.atrributetable);

	TreeNode root;
	root = createTreeDriver(atrributetable, names, labels);
	printTreeDriver(&root);
	return 0;
}
