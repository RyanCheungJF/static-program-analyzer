#include "Constant.h"

Constant::Constant(int val) {
	value = val;
}

void Constant::accept(ASTVisitor* visitor) const {
	visitor->visitExpression(this);
}