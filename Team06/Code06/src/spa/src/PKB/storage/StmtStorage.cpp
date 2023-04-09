#include "StmtStorage.h"

void StmtStorage::writeStatement(Stmt s, StmtNum line) {
    stmt_stmtNum[AppConstants::STMT].insert(line);
    stmt_stmtNum[s].insert(line);
    stmtNum_stmt[line] = s;
    return;
}

bool StmtStorage::checkStatement(Stmt stmt, StmtNum num) {
    if (stmt_stmtNum.find(stmt) == stmt_stmtNum.end()) {
        return false;
    }
    return stmt_stmtNum[stmt].find(num) != stmt_stmtNum[stmt].end();
}

std::unordered_set<StmtNum>& StmtStorage::getStatementNumbers(Stmt s) {
    if (stmt_stmtNum.find(s) == stmt_stmtNum.end()) {
        return emptyStmtNum;
    }
    return stmt_stmtNum[s];
}

Stmt StmtStorage::getStatementType(StmtNum num) {
    if (stmtNum_stmt.find(num) == stmtNum_stmt.end()) {
        return "";
    }
    return stmtNum_stmt[num];
}
