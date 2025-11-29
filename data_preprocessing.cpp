#include "data_preprocessing.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

// Constructor
DataPreprocessing::DataPreprocessing(const string& filePath)
    : filePath(filePath) {
}

// Trim whitespace from string
string DataPreprocessing::trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

string Review::trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Extract string value from JSON
string Review::extractString(const string& json, const string& key) {
    string searchKey = "\"" + key + "\":";
    size_t pos = json.find(searchKey);
    if (pos == string::npos) return "";

    pos += searchKey.length();
    while (pos < json.length() && isspace(json[pos])) pos++;

    if (pos >= json.length() || json[pos] != '"') return "";

    pos++; // Skip opening quote
    size_t endPos = pos;
    while (endPos < json.length() && json[endPos] != '"') {
        if (json[endPos] == '\\') endPos++; 
        endPos++;
    }

    return json.substr(pos, endPos - pos);
}

// Extract number value from JSON
double Review::extractNumber(const string& json, const string& key) {
    string searchKey = "\"" + key + "\":";
    size_t pos = json.find(searchKey);
    if (pos == string::npos) return 0.0;

    pos += searchKey.length();
    while (pos < json.length() && isspace(json[pos])) pos++;

    string numStr;
    while (pos < json.length() && (isdigit(json[pos]) || json[pos] == '.' || json[pos] == '-')) {
        numStr += json[pos];
        pos++;
    }

    return numStr.empty() ? 0.0 : stod(numStr);
}

// Extract boolean value from JSON
bool Review::extractBool(const string& json, const string& key) {
    string searchKey = "\"" + key + "\":";
    size_t pos = json.find(searchKey);
    if (pos == string::npos) return false;

    pos += searchKey.length();
    while (pos < json.length() && isspace(json[pos])) pos++;

    return (json.substr(pos, 4) == "true");
}

// Extract object from JSON (simplified for style field)
map<string, string> Review::extractObject(const string& json, const string& key) {
    map<string, string> result;
    string searchKey = "\"" + key + "\":";
    size_t pos = json.find(searchKey);
    if (pos == string::npos) return result;

    pos += searchKey.length();
    while (pos < json.length() && isspace(json[pos])) pos++;

    if (pos >= json.length() || json[pos] != '{') return result;

    size_t endPos = json.find('}', pos);
    if (endPos == string::npos) return result;

    string objStr = json.substr(pos + 1, endPos - pos - 1);

    // Simple key-value extraction
    size_t colonPos = objStr.find(':');
    if (colonPos != string::npos) {
        size_t keyStart = objStr.find('"');
        size_t keyEnd = objStr.find('"', keyStart + 1);
        size_t valueStart = objStr.find('"', colonPos);
        size_t valueEnd = objStr.find('"', valueStart + 1);

        if (keyStart != string::npos && keyEnd != string::npos &&
            valueStart != string::npos && valueEnd != string::npos) {
            string k = objStr.substr(keyStart + 1, keyEnd - keyStart - 1);
            string v = objStr.substr(valueStart + 1, valueEnd - valueStart - 1);
            result[k] = v;
        }
    }

    return result;
}

// Load data from JSON file
vector<string> DataPreprocessing::loadData() {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filePath << endl;
        return dataStrings;
    }

    cout << "Loading data from: " << filePath << endl;
    string line;
    int lineCount = 0;
    int errorCount = 0;

    while (getline(file, line)) {
        lineCount++;
        line = trim(line);
        if (line.empty()) continue;

        try {


            Review review;
			review.parse(line);
            //reviews.push_back(review);
			dataStrings.push_back(review.toString());
            

        }
        catch (const exception& e) {
            errorCount++;
            cerr << "Error parsing line " << lineCount << ": " << e.what() << endl;
        }
    }

    file.close();

    cout << "Data loaded successfully!" << endl;
    cout << "Total records: " << dataStrings.size() << endl;
    cout << "Total lines processed: " << lineCount << endl;
    cout << "Errors encountered: " << errorCount << endl;

    return dataStrings;
}

// Clean data - remove invalid or incomplete records
void DataPreprocessing::cleanData() {
    cout << "\n--- Data Cleaning ---" << endl;
    cout << "Records before cleaning: " << dataStrings.size() << endl;

    vector<string> cleanedReviews;
    int removedCount = 0;

    //for (const auto& review : dataStrings) {
    //    if (validateRecord(review)) {
    //        cleanedReviews.push_back(review);
    //    } else {
    //        removedCount++;
    //    }
    //}

    dataStrings = cleanedReviews;

    cout << "Records after cleaning: " << dataStrings.size() << endl;
    cout << "Records removed: " << removedCount << endl;
}

// Normalize data - standardize text fields
void DataPreprocessing::normalizeData() {
    cout << "\n--- Data Normalization ---" << endl;

    for (auto& review : dataStrings) {
        trim(review);
    }

    cout << "Data normalization completed." << endl;
}

int DataPreprocessing::getNumRecords() const {
    return dataStrings.size();
}

const vector<string>& DataPreprocessing::getReviews() const {
    return dataStrings;
}

bool DataPreprocessing::validateRecord(const Review& review) {
    if (review.reviewerID.empty() || review.asin.empty()) {
        return false;
    }
    return true;
}
