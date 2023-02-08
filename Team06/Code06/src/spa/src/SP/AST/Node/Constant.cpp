#include "Constant.h"

Constant::Constant() {}

Constant::Constant(int value) {
	this->value = value;
}

void Constant::accept(ASTVisitor* visitor) {
	visitor->visitExpression(this);
}