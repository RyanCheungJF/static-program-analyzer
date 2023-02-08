#include "IfStatement.h"

void IfStatement::accept(ASTVisitor* visitor) {
	visitor->visitIfStatement(this);
}