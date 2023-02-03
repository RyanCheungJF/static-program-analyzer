#include "RelationalExpression.h"

void RelationalExpression::accept(ASTVisitor* visitor) const {
	visitor->visitConditionalExpression(this);
}

