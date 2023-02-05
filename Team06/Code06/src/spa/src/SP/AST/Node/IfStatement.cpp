#include "IfStatement.h"

void IfStatement::accept(ASTVisitor* visitor) const {
	visitor->visitIfStatement(this);
}