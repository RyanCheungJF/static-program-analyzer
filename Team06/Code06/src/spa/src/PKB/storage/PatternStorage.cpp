#include "PatternStorage.h"


void PatternStorage::writePatternNode(Node assignNode) {

}

std::vector<StmtNum> PatternStorage::getMatchingAllAssignStatements() {}

std::vector<StmtNum> PatternStorage::getMatchingRHSNoWildcard(std::string lhs, std::string rhs) {}

std::vector<StmtNum> PatternStorage::getMatchingRHSLeftWildcard(std::string lhs, std::string rhs) {}

std::vector<StmtNum> PatternStorage::getMatchingRHSRightWildcard(std::string lhs, std::string rhs) {}

std::vector<StmtNum> PatternStorage::getMatchingRHSBothWildcard(std::string lhs) {}

std::vector<StmtNum> PatternStorage::getMatchingLHSWildcardRHSNoWildcard(std::string rhs) {}

std::vector<StmtNum> PatternStorage::getMatchingLHSWildcardRHSLeftWildcard(std::string rhs) {}

std::vector<StmtNum> PatternStorage::getMatchingLHSWildcardRHSRightWildcard(std::string rhs) {}

std::vector<StmtNum> PatternStorage::getMatchingLHSWildcardRHSBothWildcard(std::string rhs) {}