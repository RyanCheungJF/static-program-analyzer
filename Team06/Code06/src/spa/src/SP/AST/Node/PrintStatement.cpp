#include "PrintStatement.h"

//PrintStatement::PrintStatement(Variable variable, int lineNumber) {
//	this->variable = variable;
//	this->lineNumber = lineNumber;
//}

PrintStatement::PrintStatement() {}

void PrintStatement::accept(ASTVisitor* visitor) const {
	visitor->visitPrintStatement(this);
}