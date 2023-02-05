#include "CallStatement.h"

void CallStatement::accept(ASTVisitor *visitor) const {
	visitor->visitCallStatement(this);
}