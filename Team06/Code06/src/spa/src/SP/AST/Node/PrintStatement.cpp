#include "PrintStatement.h"

PrintStatement::PrintStatement() {}

PrintStatement::PrintStatement(StmtNum stmtNum, Ent varName) {
	this->statementNumber = stmtNum;
	this->varName = varName;
}

void PrintStatement::accept(ASTVisitor* visitor) {
	visitor->visitPrintStatement(this);
}