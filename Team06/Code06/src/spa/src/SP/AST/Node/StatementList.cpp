#include "StatementList.h"

StatementList::StatementList() {}

StatementList::StatementList(std::vector<std::unique_ptr<Statement>> statements) {
    this->statements = std::move(statements);
}

StmtNum StatementList::getStmtNumForStmtIdx(int idx) {
    return statements[idx]->statementNumber;
}

Statement* StatementList::getStmtForStmtIdx(int idx) {
    return statements[idx].get();
}

Statement* StatementList::getLastStatement() {
    return statements.back().get();
}

StmtNum StatementList::getLastStatementNumber() {
    return statements.back()->statementNumber;
}

void StatementList::accept(ASTVisitor* visitor) {
    visitor->visitStatementList(this);
};