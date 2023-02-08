#include "ConstantStorage.h"

void ConstantStorage::writeConstant(Const c, StmtNum line) {
    constant_stmtNum[c].insert(line);
    constNames.insert(c);
    return;
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