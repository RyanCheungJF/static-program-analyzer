#include "PatternStorage.h"


void PatternStorage::writePatternNode(Node &assignNode) {
    std::string rhsVariable = assignNode.left->value;
    rhsVariable_Nodes[rhsVariable].insert(&assignNode);
    return;
}

std::vector<StmtNum> PatternStorage::getAllAssignStatements() {
    std::unordered_set<StmtNum> set;
    for (auto keyValuePair: rhsVariable_Nodes) {
        std::string rhs = keyValuePair.first;
        std::unordered_set<Node*> value = keyValuePair.second;

        for (Node* ptr: value) {
            set.insert(ptr->line);
        }
    }

    std::vector<StmtNum> res;
    for (StmtNum num: set) {
        res.push_back(num);
    }
    return res;
}

std::vector<StmtNum> PatternStorage::getMatchingRHSNoWildcard(std::string lhs, std::string rhs) {}

std::vector<StmtNum> PatternStorage::getMatchingRHSLeftWildcard(std::string lhs, std::string rhs) {}

std::vector<StmtNum> PatternStorage::getMatchingRHSRightWildcard(std::string lhs, std::string rhs) {}

std::vector<StmtNum> PatternStorage::getMatchingRHSBothWildcard(std::string lhs) {}

std::vector<StmtNum> PatternStorage::getMatchingLHSWildcardRHSNoWildcard(std::string rhs) {}

std::vector<StmtNum> PatternStorage::getMatchingLHSWildcardRHSLeftWildcard(std::string rhs) {}

std::vector<StmtNum> PatternStorage::getMatchingLHSWildcardRHSRightWildcard(std::string rhs) {}

std::vector<StmtNum> PatternStorage::getMatchingLHSWildcardRHSBothWildcard(std::string rhs) {}