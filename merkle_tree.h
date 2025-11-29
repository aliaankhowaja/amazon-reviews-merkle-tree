#pragma once
#include <string>
#include <filesystem>
#include "picosha2.h"
#include <queue>
#include <vector>
#include <list>

using namespace std;
using namespace std::filesystem;

struct MerkleNode {
    string hash;
    bool isLeaf = 0;
    MerkleNode* left;
    MerkleNode* right;
    MerkleNode* parent;
    bool deleted;
    int nodeIndex;
    MerkleNode() {
        this->hash = "";
        this->isLeaf = 0;
        this->left = nullptr;
        this->right = nullptr;
        this->parent = nullptr;
        this->nodeIndex = -1;
		this->deleted = 0;
    }
    MerkleNode(string data, int i) {
        this->isLeaf = 1;
        this->nodeIndex = i;
		this->hash.reserve(64); // reserve space for hash
        this->hash = picosha2::hash256_hex_string(data);
        this->left = nullptr;
        this->right = nullptr;
        this->parent = nullptr;
		this->deleted = 0;
    }
    MerkleNode(MerkleNode* left, MerkleNode* right) {
        this->nodeIndex = -1;
        this->isLeaf = 0;
		this->hash.reserve(64); // reserve space for hash
        string combined;
        combined.reserve(128);
		combined = left->hash + right->hash;
        this->hash = picosha2::hash256_hex_string(combined);
        this->left = left;
        this->right = right;
        this->parent = nullptr;
		this->deleted = 0;
                
    }
};

class MerkleTree {
    MerkleNode* root;
    vector<MerkleNode*> leaves;
    string name;
    void createFromData(const vector<string>& leafData);
public:
    MerkleTree(string name, const vector<string>& leafData) {
		this->name = name;
        createFromData(leafData);
    }
    MerkleNode* getRoot() const;
    void storeRoot(const string& fileName) const;
    void printTree(MerkleNode* node, int level) const;
    list<string> getProof(int leafIndex) const;
    void deleteNode(int index);
	void updateNode(int index, const string& newData);
    void insertNode(const string& data);

	void updateHashes(MerkleNode* node);
    string getName() const;
    string getRootHash() const;

};