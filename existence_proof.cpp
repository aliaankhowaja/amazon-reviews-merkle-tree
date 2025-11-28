#include "existence_proof.h"
#include "picosha2.h"
#include <iostream>
#include <list>
using namespace std;
bool PathExistenceProof::verifyPath(list<string>& proof) {
    for (size_t i = 0; i < proof.size() - 1; i += 2) {
		string left = proof.front(); proof.pop_front();
		string right = proof.front(); proof.pop_front();
		string parent = proof.front(); proof.pop_front();
        
        if (picosha2::hash256_hex_string(left + right) != parent) {
             return false; // Hash mismatch, invalid proof
        }
    }
    return true;
}