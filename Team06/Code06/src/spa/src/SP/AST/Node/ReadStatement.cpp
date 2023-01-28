#include "ReadStatement.h"

ReadStatement::ReadStatement(Variable variable, int lineNumber) {
	this->variable = variable;
	this->lineNumber = lineNumber;
};

ReadStatement::accept(ASTVisitor visitor) {
	visitor->visitReadStatement(this);
}