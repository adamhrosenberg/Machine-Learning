#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <sstream>

using namespace std;

struct point {
	vector<double> coordinates; //size 17.
	double distance; //distance from ponit to the test point.
};

vector<string> userIDs;
int userCount = 0;
vector<int> predictions;
//vector<point> distances;
vector<point> trainingData; //every point in the training file

bool comparison(point a, point b) {
	return (a.distance < b.distance);
}

double computeDistance(vector<double> p1, vector<double> p2) {
	double distance = 0;
	for (int i = 0; i < p1.size(); i++) {
		distance += sqrt((p1.at(i) - p2.at(i)) * (p1.at(i) - p2.at(i)));
	}
	return distance;
}
void createTrainingData(string filepath, bool isTest) {
	//file path is train file.

	ifstream pipein(filepath.c_str());

	//read in the file.
	for (string line; getline(pipein, line);) {
		point trainingElement;
//		vector<double> example;
		//now that we have a line, we need to create a vector. first value in examples is the label. then its each value of the attributes.
		double label = ((line.at(0) == '0') ? -1 : 1);
		trainingElement.coordinates.push_back(label);
		line = line.substr(1);

		std::vector<std::string> tokens;
		std::string token;
		std::istringstream tokenStream(line);
		while (std::getline(tokenStream, token, ':')) {
			trainingElement.coordinates.push_back(atof(token.c_str()));
		}

		trainingElement.coordinates.erase(
				trainingElement.coordinates.begin() + 1);

		trainingData.push_back(trainingElement);
	}
}

void NN(string filepath) {
	//stream in the test file. for each line of test file, stream in the train file.
	createTrainingData("Dataset/data-splits/data.train", false); //created training data vector.

	ifstream pipein(filepath.c_str());
	double right = 0;
	double wrong = 0;
	double guess = 0;

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
		example.erase(example.begin() + 1);

		for (unsigned int point = 0; point < trainingData.size(); point++) {
			//for every point in the training data
			double distance = computeDistance(example,
					trainingData.at(point).coordinates);
			trainingData.at(point).distance = distance;
		}

		//now we have the distance from the test point to every point. now we sort the distances.
		// Sort the Points by distance from p
		sort(trainingData.begin(), trainingData.end(), comparison);

		// Now consider the first k elements and only
		// two groups
		int freq1 = 0;   // Frequency of group 0
		int freq2 = 0;   // Frequency of group 1

		for (int i = 0; i < 11; i++) {
			if (trainingData.at(i).coordinates.at(0) == -1)
				freq1++;
			else if (trainingData.at(i).coordinates.at(0) == 1)
				freq2++;
		}

		int t = 0;
		if (freq1 > freq2) {
			predictions.push_back(0);
			guess = -1;
			t = 0;
		} else {
			predictions.push_back(1);
			guess = 1;
			t = 1;
		}


		cout << userIDs.at(userCount) << "," << t << endl;
		userCount++;
//		cout << guess << endl;

//		if (guess == example.at(0))
//			right++;
//		else
//			wrong++;

	}

//	cout << "Accuracy: " << (right) / (wrong + right) << " right : " << right
//			<< " wrong : " << wrong << endl;

}

int main() {

//	stream("Dataset/data-splits/data.eval.anon", true); //testing


	ifstream pipein("Dataset/data-splits/data.eval.id");

	//read in the file.
	for (string line; getline(pipein, line);) {
		//now that we have a line, we need to create a vector. first value in examples is the label. then its each value of the attributes.
		//		cout << line << endl;
		userIDs.push_back(line);
	}

	cout << "Id,Prediction" << endl;



	NN("Dataset/data-splits/data.eval.anon"); //testing

	return 0;
}
