#pragma once

#include <memory>
#include <string>
#include "ConditionalExpression.h"
#include "ASTNode.h"

class BinaryConditionalExpression : public ConditionalExpression {
public:
	std::string condOperator;
	std::unique_ptr<ConditionalExpression> lhs;
	std::unique_ptr<ConditionalExpression> rhs;

	BinaryConditionalExpression();

	BinaryConditionalExpression(std::string condOperator,
		std::unique_ptr<ConditionalExpression> lhs,
		std::unique_ptr<ConditionalExpression> rhs);

	void accept(ASTVisitor* visitor) override;
};