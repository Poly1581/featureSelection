#include <vector>
using std::vector;
#include <set>
using std::set;
#include <string>
using std::string;
#include <iostream>
using std::cin, std::cout, std::endl;
#include <fstream>
using std::ifstream;
#include <sstream>
using std::getline, std::istringstream;
#include <cmath>
using std::round;
#include <algorithm>
using std::min, std::find;
#include <limits>

//Struct to store label and features of labelled data seperately
struct labelledData {
	int label = false;
	vector<double> features = {};
	friend bool operator==(const labelledData& a, const labelledData& b) {
		return (a.label == b.label) && (a.features == b.features);
	}
};

void print(set<int> features) {
	cout << " [ ";
	for(int f:features) {
		cout << f << " ";
	}
	cout << "]";
}

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
double distance(vector<double> a, vector<double> b, set<int> features) {
	double distance = 0;
	for(int i = 0; i < a.size(); i++) {
		if(features.count(i) != 0) {
			distance += pow(a.at(i)-b.at(i),2);
		}
	}
	return sqrt(distance);
}

//Find label of nearest neighbor in data
int nearestNeighbor(vector<labelledData> data, labelledData instance, set<int> features) {
	double minDistance = std::numeric_limits<double>::max();
	int label = -1;
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
double leaveOneOut(vector<labelledData> data, set<int> features) {
	int correct = 0;
	for(labelledData instance:data) {
		if(instance.label == nearestNeighbor(data,instance,features)) {
			correct++;
		}
	}
	return correct/(double)data.size();
}

//construct empty features set and add
void forwardSelection(vector<labelledData> data) {
	int numFeatures = data.front().features.size();
	set<int> features;
	for(int s = 0; s < numFeatures; s++) {
		int bestFeature = -1;
		double bestAccuracy = -1;
		for(int f = 0; f < numFeatures; f++) {
			if(features.count(f) == 0) {
				continue;
			}
			features.insert(f);
			double accuracy = leaveOneOut(data,features);
			cout << "\tTesting feature set ";
			print(features);
			cout << ", accuracy was " << accuracy << endl;
			if(bestAccuracy < accuracy) {
				bestAccuracy = accuracy;
				bestFeature = f;
			}
			features.erase(f);
		}
		cout << "Adding feature " << bestFeature << " was best, with accuracy " << bestAccuracy << endl;
		features.insert(bestFeature);
	}
}

//construct full feature set and remove
void backwardElimination(vector<labelledData> data) {
	int numFeatures = data.front().features.size();
	set<int> features;
	for(int f = 0; f < numFeatures; f++) {
		features.insert(f);
	}
	for(int s = numFeatures; s > 0; s--) {
		int bestFeature = -1;
		double bestAccuracy = -1;
		for(int f = 0; f < numFeatures; f++) {
			if(features.count(f) == 0) {
				continue;
			}
			features.erase(f);
			double accuracy = leaveOneOut(data,features);
			cout << "\tTesting feature set ";
			print(features);
			cout << ", accuracy was " << accuracy << endl;
			if(bestAccuracy < accuracy) {
				bestAccuracy = accuracy;
				bestFeature = f;
			}
			features.insert(f);
		}
		cout << "Removing feature " << bestFeature << " was best, with accuracy " << bestAccuracy << endl;
		features.erase(bestFeature);
	}
}

//small 61, large 33
int main() {
	cout << "Enter the name of the file containing the data which you wish to analyze" << endl;
	string fileName  = "";
	cin >> fileName;
	vector<labelledData> data = loadData(fileName);
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