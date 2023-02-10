#include "BinaryConditionalExpression.h"

void BinaryConditionalExpression::accept(ASTVisitor* visitor) {
	visitor->visitConditionalExpression(this);
}