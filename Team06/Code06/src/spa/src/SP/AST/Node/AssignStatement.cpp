#include "AssignStatement.h"

void AssignStatement::accept(ASTVisitor* visitor) const {
	visitor->visitAssignStatement(this);
}