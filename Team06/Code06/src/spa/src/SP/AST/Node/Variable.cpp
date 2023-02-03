#include "Variable.h"

void Variable::accept(ASTVisitor* visitor) const {
	visitor->visitExpression(this);
};
