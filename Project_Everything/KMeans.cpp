/*
 * KMeans.cpp
 *
 *  Created on: Dec 12, 2017
 *      Author: adam
 */

#include "KMeans.h"

KMeans::KMeans() {
	// TODO Auto-generated constructor stub

}

KMeans::~KMeans() {
	// TODO Auto-generated destructor stub
}

bool KMeans::comparison(mPoint a, mPoint b) {
	return (a.distance < b.distance);
}

double KMeans::computeDistance(vector<double> p1, vector<double> p2) {
	double distance = 0;
	for (int i = 0; i < p1.size(); i++) {
		distance += sqrt((p1.at(i) - p2.at(i)) * (p1.at(i) - p2.at(i)));
	}
	return distance;
}

void KMeans::createTrainingData(string filepath, bool isTest) {
	//file path is train file.
	double right = 0;
	double wrong = 0;

	ifstream pipein(filepath.c_str());

	//read in the file.
	for (string line; getline(pipein, line);) {
		mPoint trainingElement;
//		vector<double> example;
		//now that we have a line, we need to create a vector. first value in examples is the label. then its each value of the attributes.
		double label = ((line.at(0) == '0') ? -1 : 1);
//		trainingElement.coordinates.push_back(label);
		line = line.substr(2);

		std::vector<std::string> tokens;
		std::string token;
		std::istringstream tokenStream(line);
		while (std::getline(tokenStream, token, ' ')) {
//			trainingElement.coordinates.push_back(atof(token.c_str()));
//			key = atof(token.c_str());
			//if > average at the key in discretizedfeaturevector then 1
			size_t pos = token.find(":");      // position of "live" in str

			string str3 = token.substr(pos + 1);   // get from "live" to the end
			double featValue = stod(str3);
			trainingElement.coordinates.push_back(featValue);
		}

//		trainingElement.coordinates.erase(
//				trainingElement.coordinates.begin() + 1);

		if (!isTest) {
			labels.push_back(label);
			trainingData.push_back(trainingElement);
		} else {
			//trainingElements.coordinates is the current example
			vector<double> example = trainingElement.coordinates;
			//go through the k means and see which is closest.

			double minDistance = 2147483647;
			int minClusterID = 0;
			bool guess;
			for (int mean = 0; mean < centroids.size(); mean++) {
				int distance = computeDistance(example,
						centroids.at(mean).coordinates);
				if (distance < minDistance) {
					minDistance = distance;
					minClusterID = centroids.at(mean).cluster_id;
					guess = centroids.at(mean).label;
				}
			}

			int temp;
			if (guess) {
				temp = 1;
			} else {
				temp = -1;
			}

//			cout << "my guess " << temp << " and the label is " << label << endl;

			predictions.push_back(temp);
			if (temp == label) {
				right++;
			} else {
				wrong++;
			}

		}

	}

	if (isTest) {
//		cout << "Accuracy : " << right / (right + wrong) << endl;
	}
}

void KMeans::populateCentroids() {

	//using the forgy method, pick k examples from the trainingData to act as our centroids

	srand(time(NULL));
	for (int i = 0; i < k; i++) {
		int randomRow = rand() % 2900;
		mPoint mean = trainingData.at(randomRow);
		mean.cluster_id = i;
		mean.isCluster = true;
		mean.numOfPointsInCluster = 0;
		int l = labels.at(randomRow);
		if (l == -1) {
			mean.label = false;
		} else {
			mean.label = true;
		}
		centroids.push_back(mean);
	}
}

vector<double> KMeans::add(vector<double> p1, vector<double> p2) {
	vector<double> sum;

	for (int i = 0; i < p1.size(); i++) {
		sum.push_back(p1.at(i) + p2.at(i));
	}

	return sum;
}

void KMeans::divide(vector<double> * p1, int scale) {
	for (int i = 0; i < p1->size(); i++) {
		p1->at(i) /= scale;
	}
}

void KMeans::KM() {
//	cout << trainingData.size()<<endl;

	for (int i = 0; i < centroids.size(); i++) {
		centroids.at(i).numOfPointsInCluster = 0;
	}
	//update all of the points to its closest centroid
	double minDistance = 2147483647;
	int minClusterID = 0;
	for (int item = 0; item < trainingData.size(); item++) {
		minDistance = 2147483647;
		minClusterID = 0;
		for (int mean = 0; mean < centroids.size(); mean++) {
			int distance = computeDistance(trainingData.at(item).coordinates,
					centroids.at(mean).coordinates);
//			cout << "Distance " << distance << endl;
			if (distance < minDistance) {
				minDistance = distance;
				minClusterID = centroids.at(mean).cluster_id;
//				cout << "New min distance for training example: " << item << " distance = "  << minDistance << " with cluster id of " << minClusterID << endl;
			}
			//update mean.
		}
		trainingData.at(item).cluster_id = minClusterID;
		for (int mean = 0; mean < centroids.size(); mean++) {
			if (centroids.at(mean).cluster_id == minClusterID) {
				centroids.at(mean).numOfPointsInCluster++;
//				centroids.at(mean).runningTotal
			}
		}
	}

	for (int mean = 0; mean < centroids.size(); mean++) {
//		cout << "Number of points centroid # " << mean << " has = "
//				<< centroids.at(mean).numOfPointsInCluster << endl;
		//update the mean
		vector<double> newMean(16, 0);

		int clusterYes = 0;
		int clusterNo = 0;

		for (int item = 0; item < trainingData.size(); item++) {
			if (trainingData.at(item).cluster_id
					== centroids.at(mean).cluster_id) {
				//if the point belong to the cluster.

				newMean = add(newMean, trainingData.at(item).coordinates);

				if (labels.at(item) == -1) {
					clusterNo++;
				} else {
					clusterYes++;
				}
			}
		}
		//now divide out the mean by the number of things you just added / total num in cluster.
		divide(&newMean, centroids.at(mean).numOfPointsInCluster);
		centroids.at(mean).coordinates = newMean;

//		cout << "cluster Yes : " << clusterYes << " and clusterNo " << clusterNo
//				<< endl;
		if (clusterYes > clusterNo) {
//			cout << "setting cluster to yes " << clusterYes << endl;
			centroids.at(mean).label = true;
		} else {
//			cout << "setting cluster to no " << clusterNo << endl;
			centroids.at(mean).label = false;
		}
	}

}

void KMeans::test() {
	createTrainingData("data-splits/data.eval.anon", true);
}

void KMeans::go() {

	//populate k centroids.
	k = 3;
	createTrainingData("data-splits/data.train", false); //created training data vector.
	populateCentroids();

//	for (int i = 0; i < 5; i++) {
	KM();

//		cout << endl;
//		for(int mean = 0; mean < centroids.size(); mean++){
//			cout << "centroid # " << mean << " coordinates: ";
//			for(int element = 0; element < centroids.at(mean).coordinates.size(); element++){
//				cout << centroids.at(mean).coordinates.at(element) << " , ";
//			}
//			cout << endl;
//		}
//	}

//	cout << labels.size() << endl;
	test();

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
		            cout << userIDs.at(i) << "," << guess << endl;
	}

//	KM("data-splits/data.eval.anon"); //testing
//	KM("data-splits/data.test"); //testing
}
