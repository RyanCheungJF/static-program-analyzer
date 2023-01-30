#include "StmtStorage.h"


void StmtStorage::writeStmt(Stmt s, StmtNum line) {

    // defensive check but might not be needed if unordered_set automatically creates a new key if it does not exist
//    if (stmt_stmtNum.find(s) == stmt_stmtNum.end()) {}
    stmt_stmtNum[s].insert(line);
//    stmtNum_stmt[line].insert(s);
    return;
}

void StmtStorage::writeStmt(Stmt s, std::vector<StmtNum> lines) {
    for (StmtNum line: lines) {
        stmt_stmtNum[s].insert(line);
//        stmtNum_stmt[line].insert(s);
    }
    return;
}

std::unordered_set<StmtNum> StmtStorage::getStmtNums(Stmt s) {
    if (stmt_stmtNum.find(s) == stmt_stmtNum.end()) {
        std::unordered_set<StmtNum> emptySet;
        return emptySet;
    }
    return stmt_stmtNum.at(s);
}
