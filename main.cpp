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

vector<vector<double>> loadData(string& fileName) {
	vector<labelledData> data;
	ifstream dataFile;
	dataFile.open(fileName);
}

vector<vector<double>> loadData(string& fileName) {
	cout << "FILE NAME: " << fileName << endl;
	vector<vector<double>> data = {};
	ifstream dataFile;
	dataFile.open(fileName);
	if(!dataFile.is_open()) {
		cout << "ERROR OPENING DATA FILE" << endl;
		return {};
	}
	string line = "";
	int numInstance = 0;
	while(getline(dataFile, line)) {
		vector<double> instance = {};
		istringstream lineStream(line);
		double value = 0;
		while(lineStream >> value) {
			instance.push_back(value);
		}
		data.push_back(instance);
	}
	return data;
}

int main() {
	cout << "Enter the name of the file containing the data which you wish to analyze" << endl;
	string fileName  = "";
	cin >> fileName;
	vector<vector<double>> data = loadData(fileName);
;}