#include "StatementList.h"

StatementList::StatementList() : statements() {}

StatementList::StatementList(std::vector<std::unique_ptr<Statement>> statements) : statements(std::move(statements)) {}

StmtNum StatementList::getStmtNumForStmtIdx(int idx) const {
    return statements[idx]->statementNumber;
}

Statement* StatementList::getStmtForStmtIdx(int idx) const {
    return statements[idx].get();
}

Statement* StatementList::getLastStatement() const {
    return statements.back().get();
}

StmtNum StatementList::getLastStatementNumber() const {
    return statements.back()->statementNumber;
}

void StatementList::accept(ASTVisitor* visitor) {
    visitor->visitStatementList(this);
};