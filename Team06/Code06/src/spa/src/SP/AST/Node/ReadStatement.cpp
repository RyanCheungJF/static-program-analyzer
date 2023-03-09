#include "ReadStatement.h"

ReadStatement::ReadStatement() : varName() {}

ReadStatement::ReadStatement(StmtNum stmtNum, Ent varName) {
    this->statementNumber = stmtNum;
    this->varName = varName;
}

void ReadStatement::accept(ASTVisitor* visitor) {
    visitor->visitReadStatement(this);
}