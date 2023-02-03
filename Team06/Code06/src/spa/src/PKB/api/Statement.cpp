# include "Statement.h"

void StatementAPI::writeStatement(Stmt s, std::vector<StmtNum> lines) {
    store.writeStatement(s, lines);
    return;
}

bool StatementAPI::checkStatement(Stmt stmt, StmtNum num) {
    return store.checkStatement(stmt, num);
}

std::unordered_set<StmtNum> StatementAPI::getStatementNumbers(Stmt stmt) {
    return store.getStatementNumbers(stmt);
}
