#include "PatternWithExprStorage.h"

#include <unordered_map>

void PatternWithExprStorage::writePattern(std::string lhs, StmtNum num, std::unique_ptr<Expression> ptr) {
    lhs_stmtNum_rhsPostfix[lhs].insert(std::make_pair(num, std::move(ptr)));
    return;
}

std::unordered_set<std::pair<int, std::unique_ptr<Expression>>, hashFunction>*
PatternWithExprStorage::getPatternWithLHS(std::string lhs) {
    if (lhs_stmtNum_rhsPostfix.find(lhs) == lhs_stmtNum_rhsPostfix.end()) {
        return nullptr;
    }
    return &lhs_stmtNum_rhsPostfix.at(lhs);
}

std::unordered_map<std::string, std::unordered_set<std::pair<int, std::unique_ptr<Expression>>, hashFunction>>*
PatternWithExprStorage::getAll() {
    return &lhs_stmtNum_rhsPostfix;
}
