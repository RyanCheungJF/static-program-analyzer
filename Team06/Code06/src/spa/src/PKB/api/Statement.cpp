# include "Statement.h"

void Statement::writeStatement(Stmt s, std::vector<StmtNum> lines) {
    store.writeStatement(s, lines);
    return;
}

bool Statement::checkStatement(Stmt stmt, StmtNum num) {
    return store.checkStatement(stmt, num);
}

std::unordered_set<StmtNum> Statement::getStatementNumbers(Stmt stmt) {
    return store.getStatementNumbers(stmt);
}
