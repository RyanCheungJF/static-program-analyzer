#include "StmtStorage.h"

void StmtStorage::writeStatement(Stmt s, std::vector<StmtNum> lines) {
    for (StmtNum line: lines) {
        stmt_stmtNum[s].insert(line);
    }
    return;
}

bool StmtStorage::checkStatement(Stmt stmt, StmtNum num) {

    // source code does not even contain this type of statement
    if (stmt_stmtNum.find(stmt) == stmt_stmtNum.end()) {
        return false;
    }
    return stmt_stmtNum[stmt].find(num) != stmt_stmtNum[stmt].end();
}

std::unordered_set<StmtNum> StmtStorage::getStatementNumbers(Stmt s) {
    if (stmt_stmtNum.find(s) == stmt_stmtNum.end()) {
        std::unordered_set<StmtNum> emptySet;
        return emptySet;
    }
    return stmt_stmtNum.at(s);
}
