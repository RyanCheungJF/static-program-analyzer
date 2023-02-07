#include "BinaryConditionalExpression.h"

BinaryConditionalExpression::BinaryConditionalExpression() {}

BinaryConditionalExpression::BinaryConditionalExpression(std::string condOperator,
	std::unique_ptr<ConditionalExpression> lhs,
	std::unique_ptr<ConditionalExpression> rhs) {
	this->condOperator = condOperator;
	this->lhs = std::move(lhs);
	this->rhs = std::move(rhs);
}

void BinaryConditionalExpression::accept(ASTVisitor* visitor) const {
	visitor->visitConditionalExpression(this);
}