#include "PatternStorage.h"

void PatternStorage::writePattern(StmtNum stmtNum, std::unordered_set<Ent>& variables) {
    for (auto var : variables) {
        varsToStmtNumMap[var].insert(stmtNum);
    }
}

std::unordered_set<StmtNum>* PatternStorage::getStmtNums(Ent var) {
    if (varsToStmtNumMap.find(var) == varsToStmtNumMap.end()) {
        return &emptyStmtNums;
    }
    return &varsToStmtNumMap[var];
}

std::pair<std::vector<StmtNum>, std::vector<Ent>> PatternStorage::getAllPairs() {
    std::vector<StmtNum> stmtNums;
    std::vector<Ent> variables;
    for (auto row : varsToStmtNumMap) {
        for (auto stmtNum : row.second) {
            stmtNums.push_back(stmtNum);
            variables.push_back(row.first);
        }
    }

    return {stmtNums, variables};
}
