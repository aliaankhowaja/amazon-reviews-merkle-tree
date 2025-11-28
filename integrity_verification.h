#pragma once
#include<string>
using namespace std;
class IntegrityVerification {
private:
    string rootFilePath;
public:
    IntegrityVerification(const string& filePath);
    bool verifyRoot(const string& computedRoot);
};