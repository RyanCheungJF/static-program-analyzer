#include "CallStatement.h"

CallStatement::CallStatement() {
}

void CallStatement::accept(ASTVisitor *visitor) const {
	visitor->visitCallStatement(this);
}