#include <iostream>
#include "data_preprocessing.h"
#include <chrono>
#include "merkle_tree.h"
#include "existence_proof.h"

using namespace std;

int main() {
    cout << "=== Merkle Tree Problem ===" << endl;
    cout << "Module 1: Data Preprocessing\n" << endl;

    DataPreprocessing dataProcessor("data_small.json");


    // Load the data
    auto start = chrono::high_resolution_clock::now();

    if (!dataProcessor.loadData()) {
        cerr << "Failed to load data. Exiting..." << endl;
        return 1;
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> loadDuration = end - start;
    cout << "Data loading time: " << loadDuration.count() << " seconds" << endl;


    // Clean the data (remove invalid records)
    auto startClean = chrono::high_resolution_clock::now();
    dataProcessor.cleanData();
    auto endClean = chrono::high_resolution_clock::now();
    chrono::duration<double> cleanDuration = endClean - startClean;
    cout << "Data cleaning time: " << cleanDuration.count() << " seconds" << endl;


    // Normalize the data
    auto startNormalize = chrono::high_resolution_clock::now();
    dataProcessor.normalizeData();
    auto endNormalize = chrono::high_resolution_clock::now();
    chrono::duration<double> normalizeDuration = endNormalize - startNormalize;
    cout << "Data normalization time: " << normalizeDuration.count() << " seconds" << endl;

    // Convert data to strings for Merkle tree
    auto startToString = chrono::high_resolution_clock::now();
    vector<string> dataStrings = dataProcessor.toString();
    auto endToString = chrono::high_resolution_clock::now();
    chrono::duration<double> toStringDuration = endToString - startToString;
    cout << "Data to string conversion time: " << toStringDuration.count() << " seconds" << endl;

    cout << "\nModule 1 Completed" << endl;

    cout << "\nSample data:" << endl;
    for (int i = 0; i < min(3, (int)dataStrings.size()); i++) {
        cout << i << ": " << dataStrings[i].substr(0, 100) << "..." << endl;
    }

    cout << "\nModule 2: Merkle Tree Construction\n" << endl;

    auto startMerkle = chrono::high_resolution_clock::now();
    MerkleTree merkleTree(dataStrings);
    MerkleNode* root = merkleTree.getRoot();
    auto endMerkle = chrono::high_resolution_clock::now();
    chrono::duration<double> merkleDuration = endMerkle - startMerkle;
    cout << "Merkle Tree Root Hash: " << root->hash << endl;
    cout << "Merkle Tree construction time: " << merkleDuration.count() << " seconds" << endl;
    merkleTree.storeRoot("data_small.txt");
    cout << "\nModule 2 Completed" << endl;

    // merkleTree.printTree(root, 0);

    cout << "\nModule 3: Existence Proof Verification\n" << endl;
            auto startProof = chrono::high_resolution_clock::now();
        list<string> proof = merkleTree.getProof(0);
        

        PathExistenceProof existenceProof;
        bool isValid = existenceProof.verifyPath(proof);
        cout << "Existence proof verification result: " << (isValid ? "Valid" : "Invalid") << endl;

        auto endProof = chrono::high_resolution_clock::now();
        chrono::duration<double> proofDuration = endProof - startProof;
        cout << "Proof verification time: " << (proofDuration.count() * 1000) << " milliseconds" << endl;

    


    return 0;
}