#include "ReadStatement.h"

ReadStatement::ReadStatement() {}

ReadStatement::ReadStatement(Variable variable, int lineNumber) {
	this->variable = variable;
	this->lineNumber = lineNumber;
};

ReadStatement::accept(ASTVisitor* visitor) const {
	visitor->visitReadStatement(this);
}