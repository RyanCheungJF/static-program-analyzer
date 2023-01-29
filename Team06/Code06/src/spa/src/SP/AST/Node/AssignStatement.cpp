#include "AssignStatement.h"

AssignStatement::AssignStatement() {}

void AssignStatement::accept(ASTVisitor* visitor) const {
	visitor->visitAssignStatement(this);
}