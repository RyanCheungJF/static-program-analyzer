#include "ReadStatement.h"

ReadStatement::ReadStatement() {}

ReadStatement::ReadStatement(std::string varName) {
	this->varName = varName;
}

void ReadStatement::accept(ASTVisitor* visitor) const {
	visitor->visitReadStatement(this);
}