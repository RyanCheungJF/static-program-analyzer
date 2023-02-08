#include "WhileStatement.h"

void WhileStatement::accept(ASTVisitor* visitor) {
	visitor->visitWhileStatement(this);
}