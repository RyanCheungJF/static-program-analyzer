#pragma once

#include <iostream>
#include "Expression.h"

class MathExpression : public Expression {
public:
	std::string mathOperator; 
	std::unique_ptr<Expression> lhs;
	std::unique_ptr<Expression> rhs;

	void accept(ASTVisitor* visitor) const override;
};