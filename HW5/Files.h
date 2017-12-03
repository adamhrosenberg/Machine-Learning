/*
 * Files.h
 *
 *  Created on: Dec 2, 2017
 *      Author: adam
 */
#include <stdlib.h>
#include <iostream>
#include <random>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>
#include <sstream>
#include <map>
#include <set>


#ifndef FILES_H_
#define FILES_H_


using namespace std;

class Files {
public:
	Files();
	vector<string> trainingFiles = {
			"data/CVSplits/training00.data", "data/CVSplits/training01.data",
			"data/CVSplits/training02.data", "data/CVSplits/training03.data",
			"data/CVSplits/training04.data" };

	vector<string> testingFiles = {
			"data/speeches.train.liblinear",
			"data/speeches.test.liblinear"

	};
	virtual ~Files();
};



#endif /* FILES_H_ */
