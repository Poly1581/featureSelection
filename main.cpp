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
		cout << f+1 << " ";
	}
	cout << "]";
}

//Load labelled data from file
vector<labelledData> loadData() {
	cout << "Would you like to analyze small or large data?" << endl;
	string size = "";
	cin >> size;
	cout << "What is the data number?" << endl;
	string number = "";
	cin >> number;
	string fileName = "CS170_";
	if(size == "small") {
		fileName += "Small_Data__";
	} else if(size == "large") {
		fileName += "Large_Data__";
	}
	fileName += number+".txt";
	vector<labelledData> data;
	ifstream dataFile;
	dataFile.open(fileName);
	if(!dataFile.is_open()) {
		cout << "ERROR OPENING DATA FILE WITH NAME " << fileName << endl;
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
	cout << "FILE HAS " << data.size() << " INSTANCES" << endl;
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
	set<int> bestFeatures;
	double overallBestAccuracy = -1;
	for(int s = 0; s < numFeatures; s++) {
		int bestFeature = -1;
		double bestAccuracy = -1;
		for(int f = 0; f < numFeatures; f++) {
			if(features.count(f) != 0) {
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
		features.insert(bestFeature);
		cout << "Feature set ";
		print(features);
		cout << " was best, with accuracy " << bestAccuracy << endl;
		if(overallBestAccuracy < bestAccuracy) {
			overallBestAccuracy = bestAccuracy;
			bestFeatures = features;
		}
	}
	cout << "Feature set ";
	print(bestFeatures);
	cout << " was the best overall, with accuracy " << overallBestAccuracy << endl;
}

//construct full feature set and remove
void backwardElimination(vector<labelledData> data) {
	int numFeatures = data.front().features.size();
	set<int> features;
	for(int f = 0; f < numFeatures; f++) {
		features.insert(f);
	}
	set<int> bestFeatures = features;
	double overallBestAccuracy = leaveOneOut(data,features);
	cout << "Full feature set ";
	print(features);
	cout << " has accuracy " << overallBestAccuracy << endl;
	for(int s = numFeatures; s > 1; s--) {
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
		features.erase(bestFeature);
		cout << "Feature set ";
		print(features);
		cout << " was best, with accuracy " << bestAccuracy << endl;
		if(overallBestAccuracy < bestAccuracy) {
			overallBestAccuracy = bestAccuracy;
			bestFeatures = features;
		}
	}
	cout << "Feature set ";
	print(bestFeatures);
	cout << " was the best overall, with accuracy " << overallBestAccuracy << endl;
}

//small 61, large 33
int main() {
	vector<labelledData> data = loadData();
	cout << "RUNNING FORWARDS ELIMINATION" << endl;
	forwardSelection(data);
	cout << "RUNNING BACKWARDS ELIMINATION" << endl;
	backwardElimination(data);
}