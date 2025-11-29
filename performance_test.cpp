#include "performance_test.h"
#include "data_preprocessing.h"
#include <iostream>
#include<math.h>
using namespace std;
PerformanceTest::PerformanceTest(const string& dataFile, const string& reportFile)
{
	dataFilePath = dataFile;
	reportFilePath = reportFile;
	merkleTree = nullptr;

}



void PerformanceTest::runTests()
{
	printDataLoadingTimes();
	printBuildTime();
	printHashTimes();
	printProofTimes();
}

void PerformanceTest::printDataLoadingTimes() {
	auto start = chrono::high_resolution_clock::now();
	DataPreprocessing p(dataFilePath);
	nodeData = p.loadData();
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<float> duration = end - start;
	cout << "Data loading time: " << duration.count() << " seconds" << endl;
}

void PerformanceTest::printHashTimes() const
{
	auto start = chrono::high_resolution_clock::now();
	int s = min(50, (int)nodeData.size());
	for (int i = 0; i < s; i++)
	{
		picosha2::hash256_hex_string(nodeData[i]);
	}
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<float> duration = end - start;
	cout << "Average hash time: " << duration.count() / s << "seconds" << endl;
}

void PerformanceTest::printBuildTime() 
{
	auto start = chrono::high_resolution_clock::now();
	merkleTree = new MerkleTree(dataFilePath, nodeData);
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<float> duration = end - start;
	cout << "Merkle Tree build time: " << duration.count() << " seconds" << endl;

}

void PerformanceTest::printProofTimes() const
{
	if (!merkleTree) {
		cout << "Tree not created. Call printBuildTime() first to create the tree.";
	}
	int s = min(50, (int)nodeData.size());
	auto start = chrono::high_resolution_clock::now();

	for (int i = 0; i< s; i++)
	{
		merkleTree->getProof(i);
	}
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<float> duration = end - start;
	cout << "Average proof generation time: " << duration.count() / s * 1000 << "ms" << endl;
}
