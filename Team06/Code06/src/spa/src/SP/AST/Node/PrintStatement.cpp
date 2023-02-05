#include "PrintStatement.h"

PrintStatement::PrintStatement() {}

PrintStatement::PrintStatement(std::string varName) {
	this->varName = varName;
}

void PrintStatement::accept(ASTVisitor* visitor) const {
	visitor->visitPrintStatement(this);
}