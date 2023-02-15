#include "ConstantStorage.h"

void ConstantStorage::writeConstant(StmtNum num, std::unordered_set<Const> constants) {
    stmtNum_constant[num].insert(constants.begin(), constants.end());
    for (Const c : constants) {
        constant_stmtNum[c].insert(num);
    }
    for (Const c : constants) {
        constNames.insert(c);
    }
}

bool ConstantStorage::checkConstant(Const c, StmtNum num) {
    // source code does not even contain this constant
    if (constant_stmtNum.find(c) == constant_stmtNum.end()) {
        return false;
    }
    return constant_stmtNum[c].find(num) != constant_stmtNum[c].end();
}

std::unordered_set<StmtNum> ConstantStorage::getConstantStmtNums(Const c) {
    if (constant_stmtNum.find(c) == constant_stmtNum.end()) {
        std::unordered_set<StmtNum> emptySet;
        return emptySet;
    }
    return constant_stmtNum.at(c);
}

std::unordered_set<Const> ConstantStorage::getConstNames() {
    return constNames;
}