#include "Constant.h"

void Constant::accept(ASTVisitor* visitor) const {
	visitor->visitExpression(this);
}