#include <list>
using std::list;
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
#include <algorithm>
using std::min;
using std::iterator;


//Struct to store label and features of labelled data seperately
struct labelledData {
	double label = 0;
	vector<double> features = {};
};


//Load labelled data from file
list<labelledData> loadData(string& fileName) {
	//Make list of labelled data and input stream to read from
	list<labelledData> data;
	ifstream dataFile;
	dataFile.open(fileName);


	if(!dataFile.is_open()) {
		cout << "ERROR OPENING DATA FILE" << endl;
		return {};
	}


	string line = "";
	double label = 0;
	double feature = 0;
	//Read data line by line and add instances to vector
	while(getline(dataFile, line)) {
		labelledData instance;
		istringstream lineStream(line);
		lineStream >> label;
		instance.label = label;
		while(lineStream >> feature) {
			instance.features.push_back(feature);
		}
		data.push_back(instance);
	}
	return data;
}

double distance(vector<double> a, vector<double> b, list<int> features) {
	if(a.size() != b.size()) {
		cout << "MISMATCHING SIZES" << endl;
		return 0;
	}
	double distance = 0;
	for(int i = 0; i < a.size(); i++) {
		if()
		distance += pow(abs(a.at(i)-b.at(i)),2);
	}
	return sqrt(distance);
}

double nearestNeighbor(list<labelledData> data, vector<double> unlabelled, list<int> features) {
	double label = -1;
	double minDistance = -1;
	for(labelledData labelled:data) {
		double currDistance = distance(labelled.features,unlabelled,features);
		minDistance = min(minDistance,currDistance);
		label = labelled.label;
	}
	return label;
}

double leaveOneOut(list<labelledData> data, list<int> features) {
	int correct = 0;
	int total = data.size();
	for(int i = 0; i < total; i++) {
		//Remove front element
		labelledData current = data.front();
		data.pop_front();
		//Find label of nearest neighbor
		double label = nearestNeighbor(data,current.features,features);
		if(label == current.label) {
			correct++;
		}
		//Reinsert current element
		data.push_back(current);
	}
	return (double)correct/(double)total;
}

int main() {
	cout << "Enter the name of the file containing the data which you wish to analyze" << endl;
	string fileName  = "";
	cin >> fileName;
	list<labelledData> data = loadData(fileName);

	// for(labelledData instance:data) {
	// 	cout << "LABEL: " << instance.label << endl;
	// 	cout << "FEATURES: ";
	// 	for(double feature:instance.features) {
	// 		cout << feature << " ";
	// 	}
	// 	cout << endl;
	// }
	// cout << endl;
}