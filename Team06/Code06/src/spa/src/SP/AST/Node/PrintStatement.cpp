#include "PrintStatement.h"

PrintStatement::PrintStatement(std::string varName) {
	this->varName = varName;
}

PrintStatement::PrintStatement() {}

void PrintStatement::accept(ASTVisitor* visitor) const {
	visitor->visitPrintStatement(this);
}