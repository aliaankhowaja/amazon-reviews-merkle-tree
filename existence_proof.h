#pragma once 
#include<string>
#include<list>

using namespace std;

class PathExistenceProof {
public:
    PathExistenceProof() {}
    bool verifyPath(list<string>& proof);
};