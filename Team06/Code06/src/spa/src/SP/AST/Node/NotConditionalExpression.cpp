#include "NotConditionalExpression.h"

void NotConditionalExpression::accept(ASTVisitor* visitor) {
	visitor->visitConditionalExpression(this);
}