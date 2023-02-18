#include "Constant.h"

Constant::Constant() {}

Constant::Constant(Const value) {
	this->value = value;
}

void Constant::accept(ASTVisitor* visitor) {
	visitor->visitExpression(this);
}