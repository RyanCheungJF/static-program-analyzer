#include "StatementList.h"

StatementList::StatementList() {}

StatementList::StatementList(std::vector<std::unique_ptr<Statement>> statements) {
    this->statements = std::move(statements);
}

StmtNum StatementList::getStmtNumForStmtIdx(int idx) {
    return this->statements[idx]->statementNumber;
}

void StatementList::accept(ASTVisitor* visitor) {
    visitor->visitStatementList(this);
};