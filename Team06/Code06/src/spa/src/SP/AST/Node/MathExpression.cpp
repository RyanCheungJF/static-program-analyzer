#include "MathExpression.h"

void MathExpression::accept(ASTVisitor* visitor) const {
	visitor->visitExpression(this);
}