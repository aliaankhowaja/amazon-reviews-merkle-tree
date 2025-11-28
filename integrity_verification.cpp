#include "integrity_verification.h"
#include<iostream>
#include<fstream>

using namespace std;

IntegrityVerification::IntegrityVerification(const string& filePath)
    : rootFilePath(filePath) {
}

bool IntegrityVerification::verifyRoot(const string& computedRoot) {
    ifstream file(rootFilePath);
    if (!file.is_open()) {
        cerr << "Error: Unable to open root file: " << rootFilePath << endl;
        return false;
    }

    string storedRoot;
    while (getline(file, storedRoot)); // Read the last stored root
    if (storedRoot.empty()) {
        cerr << "Error: No stored roots found in file." << endl;
        return false;
    }

    if (computedRoot == storedRoot) {
        cout << "Integrity Verified: Computed root matches stored root." << endl;
        return true;
    } else {
        cout << "Integrity Verification Failed: Computed root does not match stored root." << endl;
        return false;
    }
}