#include "WhileStatement.h"

void WhileStatement::accept(ASTVisitor* visitor) const {
	visitor->visitWhileStatement(this);
}