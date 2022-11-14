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
#include <random>
#include <chrono>

//Struct to store label and features of labelled data seperately
struct labelledData {
	bool label = false;
	vector<double> features = {};
	friend bool operator==(const labelledData& a, const labelledData& b) {
		return (a.label == b.label) && (a.features == b.features);
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
		if((int)label == 1) {
			instance.label = false;
		} else {
			instance.label = true;
		};
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
bool nearestNeighbor(vector<labelledData> data, labelledData instance, vector<int> features) {
	double minDistance = std::numeric_limits<double>::max();
	bool label = false;
	for(labelledData neighbor:data) {
		if(neighbor == instance) {
			continue;
		}
		double neighborDistance = distance(neighbor.features, instance.features, features);
		if(neighborDistance <= minDistance) {
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

void displayVector(vector<int> features) {
	cout << "{";
	for(int f = 0; f < features.size()-1; f++) {
		cout << features.at(f) << ", ";
	}
	cout << features.back() << "}";
}

void displayResult(vector<int> features, double accuracy) {
	cout << "\tAccuracy using feature(s) ";
	displayVector(features);
	cout << " was " << round(accuracy*1000)/10 << endl;
}

void displayBest(vector<int> features, double accuracy) {
	cout << "Accuracy of feature set ";
	displayVector(features);
	cout << " was best (" << round(accuracy*1000)/10 << ")" << endl;
}

//start with emtpy feature set and add
void forwardSelection(vector<labelledData> data) {
	vector<int> dataFeatures = {};
	for(int f = 0; f < data.front().features.size(); f++) {
		dataFeatures.push_back(f);
	}
	vector<int> features = {};
	while(features.size() < dataFeatures.size()) {
		double maxAccuracy = -1;
		int bestFeature = -1;
		for(int feature:dataFeatures) {
			if(find(features.begin(), features.end(), feature) == features.end()) {
				vector<int> newFeatures = features;
				newFeatures.push_back(feature);
				double accuracy = leaveOneOut(data,newFeatures);
				displayResult(newFeatures,accuracy);
				if(accuracy > maxAccuracy) {
					maxAccuracy = accuracy;
					bestFeature = feature;
				}
			}
		}
		features.push_back(bestFeature);
		displayBest(features,maxAccuracy);
	}
}

//start with full feature set and remove
void backwardElimination(vector<labelledData> data) {

}

int main() {
	cout << "Enter the name of the file containing the data which you wish to analyze" << endl;
	string fileName  = "";
	cin >> fileName;
	vector<labelledData> data = loadData(fileName);
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(data.begin(), data.end(), std::default_random_engine(seed));
	cout << "FILE HAS " << data.size() << " INSTANCES" << endl;
	forwardSelection(data);
	// for(labelledData instance:data) {
	// 	if(instance.label) {
	// 		cout << "LABEL: TRUE" << endl;
	// 	} else {
	// 		cout << "LABEL: FALSE" << endl;
	// 	}
	// 	for(double feature:instance.features) {
	// 		cout << "\tFEATURE: " << feature << endl;
	// 	}
	// }
	// cout << "ACCURACY USING FIRST FEATURE: " << leaveOneOut(data,{0}) << endl;
	// cout << "ACCURACY USING SECOND FEATURE: " << leaveOneOut(data,{1}) << endl;
	// cout << "ACCURACY USING BOTH FEATURES: " << leaveOneOut(data,{0,1}) << endl;
}