#include "NotConditionalExpression.h"

void NotConditionalExpression::accept(ASTVisitor* visitor) const {
	visitor->visitConditionalExpression(this);
}