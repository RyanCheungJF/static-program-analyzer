#include "StmtStorage.h"

void StmtStorage::writeStatement(Stmt s, StmtNum line) {
    stmt_stmtNum[AppConstants::STMT].insert(line);
    stmt_stmtNum[s].insert(line);
    stmtNum_stmt[line].insert(s);
    return;
}

bool StmtStorage::checkStatement(Stmt stmt, StmtNum num) {
    if (stmt_stmtNum.find(stmt) == stmt_stmtNum.end()) {
        return false;
    }
    return stmt_stmtNum[stmt].find(num) != stmt_stmtNum[stmt].end();
}

std::unordered_set<StmtNum>* StmtStorage::getStatementNumbers(Stmt s) {
//    if (s == AppConstants::STMT) { //todo: explore if this can be done at compile time
//        std::unordered_set<StmtNum> res;
//        for (auto i : stmt_stmtNum) {
//            auto p = stmt_stmtNum.at(i.first);
//            for (auto v : p) {
//                res.insert(v);
//            }
//        }
//        return res;
//    }

    if (stmt_stmtNum.find(s) == stmt_stmtNum.end()) {
        return &emptyStmtNum;
    }
    return &stmt_stmtNum[s];
}

std::unordered_set<Stmt>* StmtStorage::getStatementType(StmtNum num) {
    if (stmtNum_stmt.find(num) == stmtNum_stmt.end()) {
        return &emptyStmt;
    }
    return &stmtNum_stmt[num];
}
