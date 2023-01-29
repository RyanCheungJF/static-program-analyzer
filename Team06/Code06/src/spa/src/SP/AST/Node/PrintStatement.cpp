#include "PrintStatement.h"

PrintStatement::PrintStatement(Variable variable, int lineNumber) {
	this->variable = variable;
	this->lineNumber = lineNumber;
};

PrintStatement::accept(ASTVisitor* visitor) const {
	visitor->visitPrintStatement(this);
}