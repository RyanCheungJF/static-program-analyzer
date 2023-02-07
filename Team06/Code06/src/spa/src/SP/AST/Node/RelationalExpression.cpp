#include "RelationalExpression.h"

RelationalExpression::RelationalExpression() {};

RelationalExpression::RelationalExpression(std::string relationalOperator,
	std::unique_ptr<Expression> lhs,
	std::unique_ptr<Expression> rhs) {
	this->relationalOperator = relationalOperator;
	this->lhs = std::move(lhs);
	this->rhs = std::move(rhs);
}

void RelationalExpression::accept(ASTVisitor* visitor) const {
	visitor->visitConditionalExpression(this);
}

