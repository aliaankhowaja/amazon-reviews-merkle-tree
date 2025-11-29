#pragma once
#include "merkle_tree.h"
#include <vector>
class TamperDetection {
public :
	void getTamperedNodes(MerkleNode* node1, MerkleNode *node2, vector<MerkleNode*> &tamperedNodes);
};