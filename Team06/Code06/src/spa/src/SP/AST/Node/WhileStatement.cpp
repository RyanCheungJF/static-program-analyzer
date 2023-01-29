#include "WhileStatement.h"

WhileStatement::WhileStatement() {}

void WhileStatement::accept(ASTVisitor* visitor) const {
	visitor->visitWhileStatement(this);
}