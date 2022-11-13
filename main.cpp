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

struct labelledData {
	double label = 0;
	vector<double> features = {};
};

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
		instance.label = label;
		while(lineStream >> feature) {
			instance.features.push_back(feature);
		}
		data.push_back(instance);
	}
	return data;
}

int main() {
	cout << "Enter the name of the file containing the data which you wish to analyze" << endl;
	string fileName  = "";
	cin >> fileName;
	vector<labelledData> data = loadData(fileName);
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