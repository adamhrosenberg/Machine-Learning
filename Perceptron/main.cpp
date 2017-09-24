#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;
double NORMALIZED_VALUE = 0.18257;

struct point {
	double x, y;
};

struct meanR {
	double r1 = 0, r2 = 0, r3 = 0;
};

struct perceptronReturn {
	double accuracy;
	vector<double> weights;
	meanR rates;
	perceptronReturn(double _accuracy, vector<double> _weights, meanR _rates) {
		accuracy = _accuracy;
		weights = _weights;
		rates = _rates;
	}
};

struct crossValidateReturn {
	meanR rates;
	double accuracy;
};

vector<point> fill(string line) {
	//we have a line. now split line by spaces to have a new vector.
	line = line.substr(2);
	string buf;
	stringstream ss(line);
	vector<string> tokens;
	vector<point> row; //to return.
	vector<point> temp;

	while (ss >> buf) {
		tokens.push_back(buf);
	}

	//replace each colon with a space to seperate again.
	for (unsigned int i = 0; i < tokens.size(); i++) {
		replace(tokens.at(i).begin(), tokens.at(i).end(), ':', ' ');

		string colBuf;
		stringstream ss2(tokens.at(i));
		vector<string> line;

		while (ss2 >> colBuf) {
			line.push_back(colBuf);
		}

		//pull values from string
		double x = atoi(line.at(0).c_str());
		double y = atoi(line.at(1).c_str());

		point p;
		p.x = x;
		p.y = y;
		temp.push_back(p);
	}

	//fill an empty row.
	for (int i = 1; i < 69; i++) {
		point p;
		p.x = i;
		p.y = 0;
		row.push_back(p);
	}

	//set last element in the row to one, so the dot product with the weight vector is allowed.
	point p;
	p.x = 1;
	p.y = 1;
	row.push_back(p);

	//for each non zero value in the row set it to the row element correctly.
	for (unsigned int i = 0; i < temp.size(); i++) {
		point nonZero;
		nonZero.x = temp.at(i).x; //the index into row.
		nonZero.y = NORMALIZED_VALUE;
		row[nonZero.x - 1] = nonZero;
	}
	return row;
}

double dotP(vector<double> weights, vector<point> row) {
	double result = 0;
	for (unsigned int i = 0; i < row.size(); i++) {
		result += weights.at(i) * row.at(i).y;
	}
	return result;
}

void update(vector<double> * weights, double trainingY, vector<point> row,
		double r) {
	for (unsigned int i = 0; i < row.size(); i++) {
		//were at the bias and do b += ry;
		if (i == row.size() - 1) {
			double weight = weights->at(i) + (r * trainingY);
			weights->at(i) = weight;
		} else {
			//not at the bias. this doesn't make a difference since the corresponding feature to the bias is 1 but since its part of the algorithm i included it
			double weight = weights->at(i) + (r * trainingY * row.at(i).y);
			weights->at(i) = weight;
		}
	}
}
void perceptronHelper(vector<vector<point> > matrix,
		vector<int> labels, double r, vector<double> weights, int epoch,
		bool compareDev) {

	double numRight = 0;
	double numWrong = 0;
	double trainingY = 0;
	double accuracy = 0;

	meanR epochMean;
	//for each example in the training set

	for (int num = 0; num < epoch; num++) {
		for (unsigned int i = 0; i < matrix.size(); i++) {
			//make a prediction and compare with training Y
			trainingY = labels.at(i); //the label that comes with the data
			//take the dot product of the weights matrix and the row of the matrix we're currently in
			double prediction = dotP(weights, matrix.at(i));
			//add this prediction to the prediction vector to check accuracy.
			if (prediction > 0) {
				prediction = 1;
			} else if (prediction <= 0) {
				prediction = -1;
			}

			if (prediction != trainingY) {
				numWrong++;
				update(&weights, trainingY, matrix.at(i), r);
			} else {
				numRight++;
			}
		}
		accuracy = numRight / (numRight + numWrong);
		cout << accuracy << "%, ";
	}
}

perceptronReturn basicPerceptron(vector<vector<point> > matrix,
		vector<int> labels, double r, vector<double> weights, int epoch,
		int compareDev) {

	double numRight = 0;
	double numWrong = 0;
	double trainingY = 0;
	double accuracy = 0;

	meanR epochMean;
	//for each example in the training set
	for (int _epoch = 0; _epoch < epoch; _epoch++) {
		for (unsigned int i = 0; i < matrix.size(); i++) {
			//make a prediction and compare with training Y
			trainingY = labels.at(i); //the label that comes with the data
			//take the dot product of the weights matrix and the row of the matrix we're currently in
			double prediction = dotP(weights, matrix.at(i));
			//add this prediction to the prediction vector to check accuracy.
			if (prediction > 0) {
				prediction = 1;
			} else if (prediction <= 0) {
				prediction = -1;
			}

			if (prediction != trainingY) {
				numWrong++;
				update(&weights, trainingY, matrix.at(i), r);
			} else {
				numRight++;
			}
		}

		accuracy = numRight / (numRight + numWrong);

		if (r == 1) {
			epochMean.r1 += accuracy;
		} else if (r == 0.1 || r == 0.1 / (1 + 0.01)) {
			epochMean.r2 += accuracy;
		} else if (r == 0.01 || r == 0.01 / (1 + 0.02)) {
			epochMean.r3 += accuracy;
		}

	}

	//after each epoch compare to the dev set if needed
	if (compareDev == 1) {
		//use the weights for the dev set.
		string filepath = "Dataset/phishing.dev"; //store path since the pipe consumes that string.
		ifstream pipein(filepath.c_str());

		vector<string> tempLinesDev; //used to temp store the file to go through it.
		vector<vector<point> > matrixDev; //matrix of input data.
		vector<int> labelsDev; // -1, +1 label vector.

		//read in the file. push file lines to another vector and handle the labels. O(2n) to make it look a little cleaner
		for (string line; getline(pipein, line);) {
			tempLinesDev.push_back(line);
			if (line.at(0) == '+') {
				labelsDev.push_back(1);
			} else {
				labelsDev.push_back(-1);
			}
		}

		//for each line of the file. create a row of the matrix based on the data. add that row to the matrix. each row is 67 long. 0-66. first x in the 0th element has a value of 1.
		for (unsigned int i = 0; i < tempLinesDev.size(); i++) {
			vector<point> row = fill(tempLinesDev.at(i));
			matrixDev.push_back(row);
		}
		//epoch = 20 for this.
		perceptronHelper(matrixDev, labelsDev, r, weights, 20, 0);
	} else if (compareDev == 2) {
		//running against the training set
		string filepath = "Dataset/phishing.test"; //store path since the pipe consumes that string.
		ifstream pipein(filepath.c_str());

		vector<string> tempLinesDev; //used to temp store the file to go through it.
		vector<vector<point> > matrixDev; //matrix of input data.
		vector<int> labelsDev; // -1, +1 label vector.

		//read in the file. push file lines to another vector and handle the labels. O(2n) to make it look a little cleaner
		for (string line; getline(pipein, line);) {
			tempLinesDev.push_back(line);
			if (line.at(0) == '+') {
				labelsDev.push_back(1);
			} else {
				labelsDev.push_back(-1);
			}
		}

		//for each line of the file. create a row of the matrix based on the data. add that row to the matrix. each row is 67 long. 0-66. first x in the 0th element has a value of 1.
		for (unsigned int i = 0; i < tempLinesDev.size(); i++) {
			vector<point> row = fill(tempLinesDev.at(i));
			matrixDev.push_back(row);
		}
		perceptronHelper(matrixDev, labelsDev, r, weights, 1, 0);
	}

	meanR mean;
	mean.r1 = epochMean.r1 / epoch;
	mean.r2 = epochMean.r2 / epoch;
	mean.r3 = epochMean.r3 / epoch;
	perceptronReturn pr(accuracy, weights, mean);
	return pr;
}

perceptronReturn perceptronDriver(string path, vector<double> weights, double r,
		int epoch, int compareDev) {
	string filepath = path; //store path since the pipe consumes that string.
	ifstream pipein(path.c_str());

	vector<string> tempLines; //used to temp store the file to go through it.
	vector<vector<point> > matrix; //matrix of input data.
	vector<int> labels; // -1, +1 label vector.

	//read in the file. push file lines to another vector and handle the labels. O(2n) to make it look a little cleaner
	for (string line; getline(pipein, line);) {
		tempLines.push_back(line);
		if (line.at(0) == '+') {
			labels.push_back(1);
		} else {
			labels.push_back(-1);
		}
	}

	//for each line of the file. create a row of the matrix based on the data. add that row to the matrix. each row is 67 long. 0-66. first x in the 0th element has a value of 1.
	for (unsigned int i = 0; i < tempLines.size(); i++) {
		vector<point> row = fill(tempLines.at(i));
		matrix.push_back(row);
	}

	//call perceptron
	perceptronReturn pr = basicPerceptron(matrix, labels, r, weights, epoch,
			compareDev);

	return pr;
}

crossValidateReturn crossValidate(vector<string> files, int against, double r,
		int epoch) {
	//against is the file we are testing against

	//creating strings for nice output.
	string against0 = "1, 2, 3, 4";
	string against1 = "0, 2, 3, 4";
	string against2 = "0, 1, 3, 4";
	string against3 = "0, 1, 2, 4";
	string against4 = "0, 1, 2, 3";
	//fill in weights w/ random values to start the cross validation.
	vector<double> weights;
	srand(time(NULL));
	for (int i = 0; i < 69; i++) {
		double randomInitValue = rand() % (20000) - (10000);
		randomInitValue /= 100000;
		weights.push_back(randomInitValue);
	}

	//for each file we have, training on the files except for the one we're testing against.
	meanR mean;
	for (unsigned int i = 0; i < files.size(); i++) {
		if ((signed) i != against) {
			perceptronReturn pr(
					perceptronDriver(files.at(i), weights, r, epoch, false));
			weights = pr.weights;
			mean = pr.rates;
		}
	}
	//data is all trained. now test against against.
	perceptronReturn pr(
			perceptronDriver(files.at(against), weights, r, epoch, false));
	string training;
	if (against == 0) {
		training = against0;
	} else if (against == 1) {
		training = against1;
	} else if (against == 2) {
		training = against2;
	} else if (against == 3) {
		training = against3;
	} else if (against == 4) {
		training = against4;
	}
//	cout
//			<< "Training complete. Testings the model (weights) generated from files "
//			<< training << " against: " << files.at(against) << " = "
//			<< pr.accuracy << "% accuracy" << endl;
	weights = pr.weights;
	crossValidateReturn re;
	re.rates = mean;
	re.accuracy = pr.accuracy;
	return re;
}

void printTrainingAverages(double trainedAgainst0, double trainedAgainst1,
		double trainedAgainst2, double trainedAgainst3,
		double trainedAgainst4) {
	cout << "\nThe average accuracy while training against training file 0: "
			<< trainedAgainst0 / 3
			<< "\nThe average accuracy while training against training file 1: "
			<< trainedAgainst1 / 3
			<< "\nThe average accuracy while training against training file 2: "
			<< trainedAgainst2 / 3
			<< "\nThe average accuracy while training against training file 3: "
			<< trainedAgainst3 / 3
			<< "\nThe average accuracy while training against training file 4: "
			<< trainedAgainst4 / 3 << "\n" << endl;
}

//prints the averages. returns the max.
point printAverageRateAccuracy(double r1, double r2, double r3, string testName,
		int num) {
	double r1Percentage = r1 / num;
	double r2Percentage = r2 / num;
	double r3Percentage = r3 / num;

	cout << testName << "Mean accuracy with a learning rate of 1 = "
			<< r1Percentage
			<< "\t Mean accuracy with a learning rate of 0.1 =  "
			<< r2Percentage
			<< "\t Mean accuracy with a learning rate of 0.01 =  "
			<< r3Percentage << endl;

	double max = (r1Percentage < r2Percentage) ? r2Percentage : r1Percentage;
	double highestAccuracy = ((max < r3Percentage) ? r3Percentage : max);

	point toReturn; //learning rate, accuracy
	toReturn.y = highestAccuracy;
	if (highestAccuracy == r1Percentage) {
		toReturn.x = r1;
	} else if (highestAccuracy == r2Percentage) {
		toReturn.x = r2;
	} else if (highestAccuracy == r3Percentage) {
		toReturn.x = r3;
	}
	return toReturn;
}

void simplePerceptronDriver(int sectionNum) {
	vector<string> files;
	files.push_back("Dataset/CVSplits/training00.data");
	files.push_back("Dataset/CVSplits/training01.data");
	files.push_back("Dataset/CVSplits/training02.data");
	files.push_back("Dataset/CVSplits/training03.data");
	files.push_back("Dataset/CVSplits/training04.data");

	vector<double> learningRate;
	learningRate.push_back(1);
	learningRate.push_back(0.1);
	learningRate.push_back(0.01);

	double t = 0;

	int epoch;
	double r1Sum = 0, r2Sum = 0, r3Sum = 0; //1, .1, .01
	double trainedAgainst0 = 0, trainedAgainst1 = 0, trainedAgainst2 = 0,
			trainedAgainst3 = 0, trainedAgainst4 = 0;

	double epochAverager1 = 0, epochAverager2 = 0, epochAverager3 = 0;
	double optimalLearningRate = 0.0;

	/*
	 * STARTING BASIC PERCEPTRON VALIDATION.
	 */
	//doing basic perceptron 5 fold validation with each learning rate starting with an epoch of 10.
	cout
			<< "Beginning 5 fold validation for the basic perceptron. Each file is tested against the trained model from the other 4 files.\n"
			<< "Throughout each validation, the accuracy for each learning rate is averaged and printed at the end. The current epoch is ten.\n"
			<< "Here we are finding the optimal value of r to use for the rest of the experiment."
			<< endl;
	cout << "Testing with 10 epochs" << endl;
	epoch = 10;
	for (unsigned int j = 0; j < learningRate.size(); j++) {
		if(sectionNum == 1){
			cout << "Validating with learning rate of : " << learningRate.at(j)
					<< endl;
		}else if(sectionNum == 2){
			//dynamic
			cout << "t = " << t << " , Validating with learning rate of : " << learningRate.at(j) / ( 1 + t) << endl;
		}

		for (unsigned int i = 0; i < files.size(); i++) {
			crossValidateReturn ret;
			//i is the file we're going to test against. j is the learning rate.
			if(sectionNum == 1){
				ret = crossValidate(files, i,
						learningRate.at(j), epoch);
			}else if(sectionNum == 2){
				ret = crossValidate(files, i,
						learningRate.at(j) / (1 + t), epoch);
			}

			//sum up the averages for all the training exapmles to give a mean later.
			if (i == 0) {
				trainedAgainst0 += ret.accuracy;
			} else if (i == 1) {
				trainedAgainst1 += ret.accuracy;
			} else if (i == 2) {
				trainedAgainst2 += ret.accuracy;
			} else if (i == 3) {
				trainedAgainst3 += ret.accuracy;
			} else if (i == 4) {
				trainedAgainst4 += ret.accuracy;
			}
			//sup up the accuracies of every example of the r to get mean later.
			r1Sum += ret.rates.r1;
			r2Sum += ret.rates.r2;
			r3Sum += ret.rates.r3;
		}
		t++;
	}

	point accuracyStruct(
			printAverageRateAccuracy(r1Sum, r2Sum, r3Sum,
					"\nAverage r accuracy from every iteration of 5 fold validation in dynamic perceptron: \n",
					5));

	optimalLearningRate = accuracyStruct.x;
	double highestAccuracy = accuracyStruct.y;

	if (optimalLearningRate == r1Sum) {
		optimalLearningRate = learningRate.at(0);
	} else if (optimalLearningRate == r2Sum) {
		optimalLearningRate = learningRate.at(1);
	} else if (optimalLearningRate == r3Sum) {
		optimalLearningRate = learningRate.at(2);
	}
	//print averages
	//maybe include which one is a max

	printTrainingAverages(trainedAgainst0, trainedAgainst1, trainedAgainst2,
			trainedAgainst3, trainedAgainst4);
	cout
			<< "\n\nFive fold validation for basic perceptron complete. The optimal learning rate is: "
			<< optimalLearningRate << " with a mean accuracy of: "
			<< highestAccuracy << "\n---" << endl;

	cout
			<< "Now that we have the optimal learning rate, we are going to use that to make a classifier from the"
			<< "training file." << endl;

	//JSING OPTIMAL R ON TRAINING FILE.
	epoch = 20;
	string trainingString = "Dataset/phishing.train";
	vector<double> weights;
	srand(time(NULL));
	for (int i = 0; i < 69; i++) {
		double randomInitValue = rand() % (20000) - (10000);
		randomInitValue /= 100000;
		weights.push_back(randomInitValue);
	}

	meanR mean;

	cout << "Testing classifier against dev set with an accuracy of: (per epoch) \n";
	perceptronReturn pr(
			perceptronDriver(trainingString, weights, optimalLearningRate,
					epoch, true));

	cout << endl;
	weights = pr.weights;
	mean = pr.rates;

	cout << "\nWe have a classifier built and are now testing against the testing set!\nAccuracay of the trained classifier against the test set: " << endl;
	//now use weights as the classifier for test.
	perceptronReturn prTest(
			perceptronDriver(trainingString, weights, optimalLearningRate,
					epoch, 2));

}
/*
 * 	Implement a Perceptron whose learning
 * 	rate decreases as η0 / (1+t)
 *	, where η0 is the starting learning rate, and t is the time step.
 *	Note that t should keep increasing across epochs. (That is, you should initialize t to 0
 *	at the start and keep incrementing for each update.)
 *	Hyper-parameter: Initial learning rate η0 ∈ {1, 0.1, 0.01}
 */
void dynamicLearningRateDrver() {
	vector<string> files;
	files.push_back("Dataset/CVSplits/training00.data");
	files.push_back("Dataset/CVSplits/training01.data");
	files.push_back("Dataset/CVSplits/training02.data");
	files.push_back("Dataset/CVSplits/training03.data");
	files.push_back("Dataset/CVSplits/training04.data");
	double t = 0; //time step to create dynamic learning rate
	vector<double> learningRate;
	learningRate.push_back(1);
	learningRate.push_back(0.1);
	learningRate.push_back(0.01);

	double r1Sum = 0, r2Sum = 0, r3Sum = 0; //1, .1, .01
	double trainedAgainst0 = 0, trainedAgainst1 = 0, trainedAgainst2 = 0,
			trainedAgainst3 = 0, trainedAgainst4 = 0;

	/*
	 * STARTING DYNAMIC PERCEPTRON VALIDATION.
	 */
	//doing basic perceptron 5 fold validation with each learning rate.
	cout
			<< "\nBeginning 5 fold validation for the Dynamic Perceptron Each file is tested against the trained model from the other 4 files.\n"
			<< "Throughout each validation, the accuracy for each learning rate is averaged and printed at the end.\n"
			<< endl;

	for (unsigned int j = 0; j < learningRate.size(); j++) {
		cout << "Validating with learning rate of : " << learningRate.at(j)
				<< "/(" << 1 << "+" << t << ")" << endl;
		for (unsigned int i = 0; i < files.size(); i++) {
			//i is the file we're going to test against. j is the learning rate.
			crossValidateReturn ret = crossValidate(files, i,
					learningRate.at(j) / (1 + t), 10);
			//sum up the averages for all the training exapmles to give a mean later.
			if (i == 0) {
				trainedAgainst0 += ret.accuracy;
			} else if (i == 1) {
				trainedAgainst1 += ret.accuracy;
			} else if (i == 2) {
				trainedAgainst2 += ret.accuracy;
			} else if (i == 3) {
				trainedAgainst3 += ret.accuracy;
			} else if (i == 4) {
				trainedAgainst4 += ret.accuracy;
			}
			//sup up the accuracies of every example of the r to get mean later.
			r1Sum += ret.rates.r1;
			r2Sum += ret.rates.r2;
			r3Sum += ret.rates.r3;
		}
		t += 0.01;
	}
	//print averages
	//maybe include which one is a max
	printAverageRateAccuracy(r1Sum, r2Sum, r3Sum,
			"\nAverage r accuracy from every iteration of 5 fold validation in dynamic perceptron: \n",
			5);
	printTrainingAverages(trainedAgainst0, trainedAgainst1, trainedAgainst2,
			trainedAgainst3, trainedAgainst4);
	cout
			<< "\n\nFive fold validation for dynamic perceptron complete. Now moving onto five fold validation for the perceptron variant with a margin.\n**********"
			<< endl;
}

int main() {

	cout << "****\nPART ONE. \n" << endl;
	simplePerceptronDriver(1);
	cout << "\n\n\n****\nPART TWO. \n" << endl;
	simplePerceptronDriver(2);

	return 0;
}
