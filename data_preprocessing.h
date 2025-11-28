#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

struct Review {
    double overall;
    string vote;
    bool verified;
    string reviewTime;
    string reviewerID;
    string asin;
    map<string, string> style;
    string reviewerName;
    string reviewText;
    string summary;
    long long unixReviewTime;

    Review() : overall(0.0), verified(false), unixReviewTime(0) {}
};

// Data Preprocessing Module class
class DataPreprocessing {
private:
    vector<Review> reviews;
    string filePath;

    string trim(const string& str);
    string extractString(const string& json, const string& key);
    double extractNumber(const string& json, const string& key);
    bool extractBool(const string& json, const string& key);
    map<string, string> extractObject(const string& json, const string& key);

public:
    DataPreprocessing(const string& filePath);

    bool loadData();
    void cleanData();
    void normalizeData();
    vector<string> toString();
    int getNumRecords() const;
    const vector<Review>& getReviews() const;

    bool validateRecord(const Review& review);
};