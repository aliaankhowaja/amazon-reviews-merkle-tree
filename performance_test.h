#pragma once

#include <string>
#include "merkle_tree.h"
using namespace std;
class PerformanceTest {
private:
	string dataFilePath;
	string reportFilePath;
	MerkleTree* merkleTree;
	vector<string> nodeData;
	void printHashTimes() const;
	void printProofTimes() const;
	void printBuildTime();
	void printDataLoadingTimes();
public:
	PerformanceTest(const string& dataFile, const string& reportFile);
	void runTests();
	
	
};