#include "BinaryConditionalExpression.h"

void BinaryConditionalExpression::accept(ASTVisitor* visitor) const {
	visitor->visitConditionalExpression(this);
}