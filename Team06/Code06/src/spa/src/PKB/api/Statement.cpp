# include "Statement.h"

void StatementAPI::writeStatement(Stmt s, std::vector<StmtNum> lines) {
    store.writeStatement(s, lines);
    return;
}

void StatementAPI::writeStatement(Stmt s, StmtNum line) {
    store.writeStatement(s, line);
    return;
}

bool StatementAPI::checkStatement(Stmt stmt, StmtNum num) {
    return store.checkStatement(stmt, num);
}

std::unordered_set<StmtNum> StatementAPI::getStatementNumbers(Stmt stmt) {
    return store.getStatementNumbers(stmt);
}
