#include "PrintStatement.h"

PrintStatement::PrintStatement() : varName() {}

PrintStatement::PrintStatement(StmtNum stmtNum, Ent varName) {
    this->statementNumber = stmtNum;
    this->varName = std::move(varName);
}

void PrintStatement::accept(ASTVisitor* visitor) {
    visitor->visitPrintStatement(this);
}