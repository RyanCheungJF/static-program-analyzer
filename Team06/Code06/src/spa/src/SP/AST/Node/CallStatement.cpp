#include "CallStatement.h"

CallStatement::CallStatement() {}

CallStatement::CallStatement(std::string procName) {
	this->procName = procName;
}

void CallStatement::accept(ASTVisitor *visitor) {
	visitor->visitCallStatement(this);
}