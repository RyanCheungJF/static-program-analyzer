#include "AssignStatement.h"

AssignStatement::AssignStatement() : varName(), expr() {}

AssignStatement::AssignStatement(StmtNum stmtNum, Ent varName, std::unique_ptr<Expression> expr) {
    this->statementNumber = stmtNum;
    this->varName = varName;
    this->expr = std::move(expr);
}

void AssignStatement::accept(ASTVisitor* visitor) {
    visitor->visitAssignStatement(this);
}