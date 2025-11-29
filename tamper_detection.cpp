#include "tamper_detection.h"

void TamperDetection::getTamperedNodes(MerkleNode* node1, MerkleNode* node2, vector<MerkleNode*>& tamperedNodes)
{
	if (node1 == nullptr || node2 == nullptr) {
		return;
	}
	if (node1->hash != node2->hash) {
		if (node1->isLeaf && node2->isLeaf) {
			tamperedNodes.push_back(node1);
			return;
		}
		getTamperedNodes(node1->left, node2->left, tamperedNodes);
		getTamperedNodes(node1->right, node2->right, tamperedNodes);
	}
}
