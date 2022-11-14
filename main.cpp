#include <vector>
using std::vector;
#include <string>
using std::string;
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <fstream>
using std::ifstream;
#include <sstream>
using std::getline;
using std::istringstream;
#include <cmath>
using std::round;
#include <algorithm>
using std::find;
#include <limits>

//Struct to store label and features of labelled data seperately
struct labelledData {
	int label = 0;
	vector<double> features = {};
	friend bool operator==(const labelledData& a, const labelledData& b) {
		if(a.label != b.label) {
			return false;
		}
		if(a.features.size() != b.features.size()) {
			return false;
		}
		for(int f = 0; f < a.features.size(); f++) {
			if(a.features.at(f) != b.features.at(f)) {
				return false;
			}
		}
		return true;
	}
};


//Load labelled data from file
vector<labelledData> loadData(string& fileName) {
	vector<labelledData> data;
	ifstream dataFile;
	dataFile.open(fileName);
	if(!dataFile.is_open()) {
		cout << "ERROR OPENING DATA FILE" << endl;
		return {};
	}
	string line = "";
	double label = 0;
	double feature = 0;
	while(getline(dataFile, line)) {
		labelledData instance;
		istringstream lineStream(line);
		lineStream >> label;
		instance.label = (int)label;
		while(lineStream >> feature) {
			instance.features.push_back(feature);
		}
		data.push_back(instance);
	}
	return data;
}


//Calculate euclidean distance between two points for given features
double distance(vector<double> a, vector<double> b, vector<int> features) {
	double distance = 0;
	for(int i = 0; i < a.size(); i++) {
		if(find(features.begin(),features.end(),i) != features.end()) {
			distance += pow(a.at(i)-b.at(i),2);
		}
	}
	return sqrt(distance);
}

//Find label of nearest neighbor in data
int nearestNeighbor(vector<labelledData> data, labelledData instance, vector<int> features) {
	double minDistance = std::numeric_limits<double>::max();
	int label = 0;
	for(labelledData neighbor:data) {
		if(neighbor == instance) {
			continue;
		}
		double neighborDistance = distance(neighbor.features, instance.features, features);
		if(neighborDistance < minDistance) {
			minDistance = neighborDistance;
			label = neighbor.label;
		}
	}
	return label;
}

//Evaluate feature accuracy
double leaveOneOut(vector<labelledData> data, vector<int> features) {
	int correct = 0;
	for(labelledData instance:data) {
		if(instance.label == nearestNeighbor(data,instance,features)) {
			correct++;
		}
	}
	return correct/(double)data.size();
}

int main() {
	cout << "Enter the name of the file containing the data which you wish to analyze" << endl;
	string fileName  = "";
	cin >> fileName;
	vector<labelledData> data = loadData(fileName);
	cout << "FILE HAS " << data.size() << " INSTANCES" << endl;
}