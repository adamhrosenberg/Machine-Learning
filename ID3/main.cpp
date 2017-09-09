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

class MatrixWrapper {
	int NUM_FEATURES = 6;
public:
	vector<vector<bool> > atrributetable;
	vector<bool> labels;
	float label_entropy = 0;
	float label_majority_error = 0;
	vector<string> names;

	int height = 0, width = 0;

	/*
	 * Constructor for Matrix class. Takes in the file
	 * to feed into the vector<vector<string>> Matrix.
	 */

	MatrixWrapper(char* file) {
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

	/*
	 0. Is their first name longer than their last name?
	 1. Do they have a middle name?
	 2. Does their first name start and end with the same letter? (ie "Ada")
	 3. Does their first name come alphabetically before their last name? (ie "Dan Klein" because "d" comes before "k")
	 4. Is the second letter of their first name a vowel (a,e,i,o,u)?
	 5. Is the number of letters in their last name even?
	 6. Is the number of letters in their first name even?
	 7. Is the first letter of their first name a vowel?
	 8. Does their last name start with a vowel?
	 9. Does the name contain a special character?
	 */
	void populateAttributeTable(vector<string> names,
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

//				attributetable.at(i).at(j) = rule6(tokens);
//				attributetable.at(i).at(j) = rule7(tokens);
//				attributetable.at(i).at(j) = rule8(tokens);
//				attributetable.at(i).at(j) = rule9(tokens);
			}
		}
//		printAttributeTable(attributetable);
	}

	void printAttributeTable(vector<vector<bool> >* atrributetable) {
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

		positivep = positivep / labels.size();
		negativep = negativep / labels.size();

		label_entropy = -positivep * log2f(positivep)
				- negativep * log2f(negativep);
		cout << "H(S) = " << label_entropy << endl;
	}

	void computerFeatureGain(vector<bool> labels,
			vector<vector<bool> > atrributetable, int featureNumber) {

		//Gain(S, Feature) = H(S) - summation |S_v| / |S| H(S_v)
		//for i < attributetable.size()

		float numberOfYes, numberOfNo;
		float positivep, negativep;
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
		float noFraction = numberOfNo/total;
		float yesFraction = numberOfYes/total;

		float secondterm = (numberOfYes - positivep) / numberOfYes;
		//positivep = positivep / numberOfYes;
		float yesTerm = positivep / numberOfYes;
		//compute for yes
		expectedYesH = -yesTerm * log2f(yesTerm) - secondterm * log2f(secondterm);
		cout << "H(S) yes = " << expectedYesH << endl;

		//now compute for no
		secondterm = (numberOfNo - negativep) / numberOfNo;
		float noTerm = negativep / numberOfNo;

		expectedNoH = -noTerm * log2f(noTerm) - secondterm * log2f(secondterm);
		cout << "H(S no) = " << expectedNoH << endl;


		gain = label_entropy - ((yesFraction*expectedYesH) + (noFraction*expectedNoH));
		cout << "Gain(Feature_" << featureNumber << "):" << gain << endl;

	}

	void computeOverallME(vector<bool> feature) {
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

		cout << "Label ME = " << label_majority_error << endl;
	}

	/*
	 *   	0. Is their first name longer than their last name?
	 */
	bool rule0(vector<string> name) {
		int first_length = name.at(0).size();
		int last_length = name.at(name.size() - 1).size();
//		cout << "first length: " << first_length << " last length: "<<last_length << endl;
		return first_length > last_length;
	}
	/*
	 * 1. Do they have a middle name?
	 */
	bool rule1(vector<string> name) {
//		cout << "middle name? " << name.size() << endl;
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

int main(int argc, char* argv[]) {
//	MatrixWrapper Matrix("Updated_Dataset/updated_train.txt");
	MatrixWrapper Matrix("Updated_Dataset/Updated_CVSplits/updated_training00.txt");
	Matrix.populateAttributeTable(Matrix.names, &Matrix.atrributetable);
	Matrix.printAttributeTable(&Matrix.atrributetable);
	Matrix.computeLabelEntropy(Matrix.labels);
	Matrix.computeOverallME(Matrix.labels);
	Matrix.computerFeatureGain(Matrix.labels, Matrix.atrributetable, 0);
//	Matrix.computerFeatureGain(Matrix.labels, Matrix.atrributetable, 1);
//	Matrix.computerFeatureGain(Matrix.labels, Matrix.atrributetable, 2);
	return 0;
}
