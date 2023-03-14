#include "Constant.h"

Constant::Constant() : value() {}

Constant::Constant(Const value) {
    this->value = value;
}

void Constant::accept(ASTVisitor* visitor) {
    visitor->visitExpression(this);
}