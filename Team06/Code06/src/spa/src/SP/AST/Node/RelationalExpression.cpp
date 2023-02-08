#include "RelationalExpression.h"

void RelationalExpression::accept(ASTVisitor* visitor) {
	visitor->visitConditionalExpression(this);
}

