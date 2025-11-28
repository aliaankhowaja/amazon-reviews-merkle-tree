#include "merkle_tree.h"
#include <iostream>
#include <list>
using namespace std;

MerkleNode* MerkleTree::getRoot() const {
    return root;
}

void MerkleTree::createFromData(const vector<string>& leafData) {
    queue<MerkleNode*> nodes;
    for (int i = 0; i < leafData.size(); i++) {
        nodes.push(new MerkleNode(leafData[i], i)); // push all the leaf merkle nodes into a queue
        leaves.push_back(nodes.back());
    }
    // build the tree
    while (nodes.size() > 1) {
        MerkleNode* left = nodes.front(); nodes.pop();
        MerkleNode* right = nodes.front(); nodes.pop();
        MerkleNode* parent = new MerkleNode(left, right);
        left->parent = parent;
        right->parent = parent;
        nodes.push(parent);
    }
    root = nodes.front();

}

void MerkleTree::storeRoot(const string& fileName) const {
    ofstream file("StoredRoots\\" + fileName, ios::app);
    // create the directory if it doesn't exist
    if (!filesystem::exists("StoredRoots")) {
        filesystem::create_directory("StoredRoots");
    }
    // append the current root hash to the file
    file << root->hash << endl;
    file.close();
}

void MerkleTree::printTree(MerkleNode* node, int level) const {
    if (node == nullptr) return;
    printTree(node->right, level + 1);
    cout << "|";
    for (int i = 0; i < level; i++) {
        if (i % 2 == 0) cout << "|";
        cout << "  ";
    }
    cout << node->hash.substr(0, 10) << (node->isLeaf ? "  " + to_string(node->nodeIndex) : "") << (node->isLeaf ? " (L)" : "") << endl;
    printTree(node->left, level + 1);
}

list<string> MerkleTree::getProof(int leafIndex) const {
    list<string> proof;
    if (leafIndex < 0 || leafIndex >= leaves.size()) {
        throw out_of_range("Leaf index out of range");
    }
    MerkleNode* current = leaves[leafIndex];
    while (current->parent != nullptr) {
        MerkleNode* parent = current->parent;
        if (parent->left == current) {
            proof.push_back(current->hash); // add current node hash
            proof.push_back(parent->right->hash); // add sibling hash
        } else {
            //cout << 
            proof.push_back(parent->left->hash); // add sibling hash
            proof.push_back(current->hash); // add current node hash
        }
		proof.push_back(parent->hash); // add parent hash
        current = parent;
    }
    return proof;
}