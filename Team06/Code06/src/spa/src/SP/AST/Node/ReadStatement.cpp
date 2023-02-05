#include "ReadStatement.h"

void ReadStatement::accept(ASTVisitor* visitor) const {
	visitor->visitReadStatement(this);
}