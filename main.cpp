#include <iostream>
#include "data_preprocessing.h"
#include <chrono>
#include "merkle_tree.h"
#include "existence_proof.h"
#include "performance_test.h"

using namespace std;

int findReviewByID(vector<string>& data, string id) {
    string rid;
    for (int i = 0; i < data.size(); i++) {
        if (data[i].substr(0, 36) == id) {
            return i;
        }
    }
    return -1;
}

int main() {
    vector<MerkleTree*> merkleTrees;
    vector<vector<string>> data;
    int option;
    string filePath;
    while (1) {
        cout << "\n================Menu================\n\n";
        cout << "1. Create Merkle Tree from data file\n";
        cout << "2. Save/Compare Merkle Roots\n";
        cout << "3. Generate Proof Paths\n";
        cout << "4. Simulate Data Tampering\n";
        cout << "5. Performance Testing\n";
        cout << "6. Exit\n";
        cout << ">> ";
        cin >> option;
        if (option == 1) {
            cout << "Enter data file path: ";
            cin >> filePath;
            DataPreprocessing dataProcessor(filePath);
            /*if (!dataProcessor.loadData()) {
                cerr << "Failed to load data. Returning to menu..." << endl;
                continue;
            }
            dataProcessor.cleanData();
            dataProcessor.normalizeData();*/
            vector<string> dataStrings = dataProcessor.loadData();
            string name;
            stringstream ss(filePath);
            getline(ss, name, '.');
            for (int i = 0; i < 3; i++) {
                cout << dataStrings[i]<<endl;
            }
            data.push_back(dataStrings);
            MerkleTree* merkleTree = new MerkleTree(name, dataStrings);
            MerkleNode* root = merkleTree->getRoot();
            cout << "Merkle Tree Root Hash: " << root->hash << endl;
            merkleTrees.push_back(merkleTree);

        }
        else if (option == 2) {
            cout << "1. Save Merkle Root\n";
            cout << "2. Compare Merkle Root\n";
            cout << ">> ";
            int o;
            cin >> o;
            if (o == 1) {
                if (merkleTrees.empty()) {
                    cout << "No Merkle Trees available. Please create one first." << endl;
                }
                else {
                    cout << "Select Merkle Tree to save root:\n";
                    for (int i = 0; i < merkleTrees.size(); i++) {
                        cout << i << ". " << merkleTrees[i]->getName() << endl;
                    }
                    int idx;
                    cin >> idx;
                    if (idx < 0 || idx >= merkleTrees.size()) {
                        cout << "Invalid index. Returning to menu." << endl;
                    }
                    else {
                        string fileName = merkleTrees[idx]->getName() + "_roots.txt";
                        merkleTrees[idx]->storeRoot(fileName);
                        cout << "Merkle Root saved to " << fileName << endl;
                    }
                }
            }
            else if (o == 2) {
                if (merkleTrees.empty()) {
                    cout << "No Merkle Trees available. Please create one first." << endl;
                }
                else {
                    cout << "Select Merkle Tree to compare root:\n";
                    for (int i = 0; i < merkleTrees.size(); i++) {
                        cout << i << ". " << merkleTrees[i]->getName() << endl;
                    }
                    cout << "ReviewID";
                    int idx;
                    cin >> idx;
                    if (idx < 0 || idx >= merkleTrees.size()) {
                        cout << "Invalid index. Returning to menu." << endl;
                    }
                    else {

                        string fileName = merkleTrees[idx]->getName() + "_roots.txt";
                        ifstream file("StoredRoots/" + fileName);
                        if (!file.is_open()) {
                            cout << "No stored roots found for this Merkle Tree." << endl;
                        }
                        else {
                            string line;
                            MerkleNode* root = merkleTrees[idx]->getRoot();
                            bool found = false;
                            while (getline(file, line)) {
                                if (line == root->hash) {
                                    found = true;
                                    break;
                                }
                            }
                            if (found) {
                                cout << "Current Merkle Root matches a stored root." << endl;
                            }
                            else {
                                cout << "Current Merkle Root does NOT match any stored roots." << endl;
                            }
                            file.close();
                        }
                    }
                }
            }
        }
        else if (option == 3) {
            if (merkleTrees.empty()) {
                cout << "No Merkle Trees available. Please create one first." << endl;
            }
            else {
                cout << "Select Merkle Tree to generate proof path:\n";
                for (int i = 0; i < merkleTrees.size(); i++) {
                    cout << i << ". " << merkleTrees[i]->getName() << endl;
                }
                int idx;
                cin >> idx;
                if (idx < 0 || idx >= merkleTrees.size()) {
                    cout << "Invalid index. Returning to menu." << endl;
                }
                else {
                    string rid;
                    cout << "Enter review id to generate proof for: ";

                    cin >> rid;
                    if (rid.size() != 36) {
                        cout << "Invalid review id. Returning to menu." << endl;
                    }
                    list<string> proof = merkleTrees[idx]->getProof(findReviewByID(data[idx], rid));
                    cout << "Proof Path:" << endl;
                    for (const string& hash : proof) {
                        cout << hash << endl;
                    }
                    PathExistenceProof existenceProof;
                    bool isValid = existenceProof.verifyPath(proof);
                    cout << "Existence proof verification result: " << (isValid ? "Valid" : "Invalid") << endl;

                }
            }
        }
        else if (option == 4) {
            if (merkleTrees.empty()) {
                cout << "No Merkle Trees available. Please create one first." << endl;
            }
            else {
                cout << "Select Merkle Tree to simulate data tampering:\n";
                for (int i = 0; i < merkleTrees.size(); i++) {
                    cout << i << ". " << merkleTrees[i]->getName() << endl;
                }
                int idx;
                cin >> idx;
                if (idx < 0 || idx >= merkleTrees.size()) {
                    cout << "Invalid index. Returning to menu." << endl;
                }
                else {
                    cout << "1. Insert Node\n";
                    cout << "2. Delete Node\n";
                    cout << "3. Update Node\n";

                    cout << ">> ";
                    int tamperOption;
                    cin >> tamperOption;
                    if (tamperOption == 1) {
                        Review newReview;
                        string newData;
                        cout << "Enter JSON data for the node: ";
                        getline(cin, newData);
                        cin.ignore();
                        newReview.parse(newData);
                        newData = newReview.toString();
                        merkleTrees[idx]->insertNode(newData);
                        cout << "Node inserted and hashes updated." << endl;
                    }
                    else if (tamperOption == 2) {
                        int leafIndex;
                        cout << "Enter leaf index to delete: ";
                        cin >> leafIndex;
                        merkleTrees[idx]->deleteNode(leafIndex);
                        cout << "Node deleted and hashes updated." << endl;
                    }
                    else if (tamperOption == 3) {
                        int leafIndex;
                        cout << "Enter leaf index to update: ";
						cin >> leafIndex;
                        Review newReview;
                        string newData;
                        cout << "Enter JSON data for the node: ";
                        getline(cin, newData);
                        newReview.parse(newData);
                        newData = newReview.toString();
                        merkleTrees[idx]->updateNode(leafIndex, newData);
                        cout << "Node updated and hashes updated." << endl;
                    }
                    else {
                        cout << "Invalid option. Returning to menu." << endl;
                    }
                }
            }
        }
        else if (option == 5) {
            cout << "Enter test data file path: ";
            cin.ignore();
            getline(cin, filePath);
			// does the file exist?
			if (!filesystem::exists(filePath)) {
				cout << "File not found. Returning to menu." << endl;
				continue;
			}
            PerformanceTest perfTest(filePath, "performance_report.txt");
            perfTest.runTests();
        }
        else if (option == 6) {
            cout << "Exiting program." << endl;
            break;
        }
        else {
            cout << "Invalid option. Please try again." << endl;
        }
    }




    //cout << "=== Merkle Tree Problem ===" << endl;
    //cout << "Module 1: Data Preprocessing\n" << endl;

    //DataPreprocessing dataProcessor("data_small.json");


    //// Load the data
    //auto start = chrono::high_resolution_clock::now();

    //if (!dataProcessor.loadData()) {
    //    cerr << "Failed to load data. Exiting..." << endl;
    //    return 1;
    //}

    //auto end = chrono::high_resolution_clock::now();
    //chrono::duration<double> loadDuration = end - start;
    //cout << "Data loading time: " << loadDuration.count() << " seconds" << endl;


    //// Clean the data (remove invalid records)
    //auto startClean = chrono::high_resolution_clock::now();
    //dataProcessor.cleanData();
    //auto endClean = chrono::high_resolution_clock::now();
    //chrono::duration<double> cleanDuration = endClean - startClean;
    //cout << "Data cleaning time: " << cleanDuration.count() << " seconds" << endl;


    //// Normalize the data
    //auto startNormalize = chrono::high_resolution_clock::now();
    //dataProcessor.normalizeData();
    //auto endNormalize = chrono::high_resolution_clock::now();
    //chrono::duration<double> normalizeDuration = endNormalize - startNormalize;
    //cout << "Data normalization time: " << normalizeDuration.count() << " seconds" << endl;

    //// Convert data to strings for Merkle tree
    //auto startToString = chrono::high_resolution_clock::now();
    //vector<string> dataStrings = dataProcessor.toString();
    //auto endToString = chrono::high_resolution_clock::now();
    //chrono::duration<double> toStringDuration = endToString - startToString;
    //cout << "Data to string conversion time: " << toStringDuration.count() << " seconds" << endl;

    //cout << "\nModule 1 Completed" << endl;

    //cout << "\nSample data:" << endl;
    //for (int i = 0; i < min(3, (int)dataStrings.size()); i++) {
    //    cout << i << ": " << dataStrings[i].substr(0, 100) << "..." << endl;
    //}

    //cout << "\nModule 2: Merkle Tree Construction\n" << endl;

    //auto startMerkle = chrono::high_resolution_clock::now();
    //MerkleTree merkleTree(dataStrings);
    //MerkleNode* root = merkleTree.getRoot();
    //auto endMerkle = chrono::high_resolution_clock::now();
    //chrono::duration<double> merkleDuration = endMerkle - startMerkle;
    //cout << "Merkle Tree Root Hash: " << root->hash << endl;
    //cout << "Merkle Tree construction time: " << merkleDuration.count() << " seconds" << endl;
    //merkleTree.storeRoot("data_small.txt");
    //cout << "\nModule 2 Completed" << endl;

    //// merkleTree.printTree(root, 0);

    //cout << "\nModule 3: Existence Proof Verification\n" << endl;
    //        auto startProof = chrono::high_resolution_clock::now();
    //    list<string> proof = merkleTree.getProof(0);
    //    

    //    PathExistenceProof existenceProof;
    //    bool isValid = existenceProof.verifyPath(proof);
    //    cout << "Existence proof verification result: " << (isValid ? "Valid" : "Invalid") << endl;

    //    auto endProof = chrono::high_resolution_clock::now();
    //    chrono::duration<double> proofDuration = endProof - startProof;
    //    cout << "Proof verification time: " << (proofDuration.count() * 1000) << " milliseconds" << endl;

    return 0;
}