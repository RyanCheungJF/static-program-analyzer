#include "MathExpression.h"

MathExpression::MathExpression() {}

MathExpression::MathExpression(std::string mathOperator, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs) {
	this->mathOperator = mathOperator;
	this->lhs = std::move(lhs);
	this->rhs = std::move(rhs);
}

void MathExpression::accept(ASTVisitor* visitor) {
	visitor->visitExpression(this);
}