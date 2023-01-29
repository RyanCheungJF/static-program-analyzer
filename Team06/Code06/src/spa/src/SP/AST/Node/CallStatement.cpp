#include "CallStatement.h"

CallStatement::CallStatement(Procedure procedure, int lineNumber) {
	this->procedure = procedure;
	this->lineNumber = lineNumber;
};

CallStatement::accept(ASTVisitor* visitor) const {
	visitor->visitCallStatement(this);
}