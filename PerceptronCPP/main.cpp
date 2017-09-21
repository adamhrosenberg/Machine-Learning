#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

struct point{
	double x, y;
};

vector<point> fill(string line){
	//we have a line. now split line by spaces to have a new vector.
	line = line.substr(2);
	string buf;
	stringstream ss(line);
	vector<string> tokens;
	vector<point> row; //to return.
	vector<point> temp;
	while(ss >> buf){
		tokens.push_back(buf);
	}

	//replace each colon with a space to seperate again.
	for(int i = 0; i < tokens.size(); i++){
		replace(tokens.at(i).begin(), tokens.at(i).end(), ':', ' ');

		string colBuf;
		stringstream ss2(tokens.at(i));
		vector<string> line;

		while(ss2 >> colBuf){
			line.push_back(colBuf);
		}

		for(int k = 0; k < line.size(); k++){
			double x = atoi(line.at(k).c_str());
			double y = 0.18257;
			if(x != 0){
				point p;
				p.x = x;
				p.y = y;
				temp.push_back(p);
			}
		}
	}

	int count = 0;
	for(int i = 0; i < temp.size(); i++){
		while(temp.at(i).x != count){
			point zeroPt;
			zeroPt.x = 0;
			zeroPt.y = 0;
			row.push_back(zeroPt);
			count++;
		}
		point nonZero;
		nonZero.x = temp.at(i).x;
		nonZero.y = 0.18257;
		row.push_back(nonZero);
	}

//	cout << "Printing row to add" << endl;
//	for(int i = 0; i < row.size(); i++){
//		cout << row.at(i).x << " " << row.at(i).y << endl;
//	}
}

int main(){

	string path = "Dataset/phishing.test";
	ifstream pipein(path.c_str());

	vector<string> tempLines;
	vector<vector<point> > matrix;
	vector<int> labels;

	for(string line; getline(pipein, line);){
		tempLines.push_back(line);
	}

	for(int i = 0; i < tempLines.size(); i++){
		vector<point> row = fill(tempLines.at(i).substr(2));
		matrix.push_back(row);
	}


	for (auto& row : matrix) {
	     for (auto& elem : row) {
	        std::cout << elem.x;
	     }
	}

	return 0;
}
