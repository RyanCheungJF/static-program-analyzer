#include "PatternStorage.h"
#include <string>
#include <unordered_map>

void PatternStorage::writePattern(std::string lhs, StmtNum num, std::unique_ptr<Expression> ptr) {
  lhs_stmtNum_rhsPostfix[lhs].insert(std::make_pair(num, std::move(ptr)));
  return;
}

std::unordered_set<std::pair<int, std::unique_ptr<Expression>>, hashFunction> *
PatternStorage::getPatternWithLHS(std::string lhs) {
  if (lhs_stmtNum_rhsPostfix.find(lhs) == lhs_stmtNum_rhsPostfix.end()) {
    return NULL;
  }
  return &lhs_stmtNum_rhsPostfix.at(lhs);
}

std::unordered_map<std::string, std::unordered_set<std::pair<int, std::unique_ptr<Expression>>, hashFunction>> *
PatternStorage::getAll() {
  return &lhs_stmtNum_rhsPostfix;
}
