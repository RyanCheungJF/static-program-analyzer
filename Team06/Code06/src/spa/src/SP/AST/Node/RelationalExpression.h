#pragma once

#include <string>
#include <memory>
#include <vector>
#include "ConditionalExpression.h"
#include "Expression.h"

class RelationalExpression : public ConditionalExpression {
public:
	std::string relationalOperator;
	std::unique_ptr<Expression> lhs;
	std::unique_ptr<Expression> rhs;

	void accept(ASTVisitor* visitor) override;
};