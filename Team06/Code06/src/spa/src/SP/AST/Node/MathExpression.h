#pragma once

#include <iostream>
#include "Expression.h"

class MathExpression : public Expression {
public:
	Operator mathOperator; 
	std::unique_ptr<Expression> lhs;
	std::unique_ptr<Expression> rhs;

	MathExpression();
	MathExpression(Operator mathOperator, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

	void accept(ASTVisitor* visitor) override;
};