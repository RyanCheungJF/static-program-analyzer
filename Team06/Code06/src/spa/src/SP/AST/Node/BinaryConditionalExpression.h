#pragma once

#include <memory>
#include <string>
#include "ConditionalExpression.h"
#include "ASTNode.h"

class BinaryConditionalExpression : public ConditionalExpression {
public:
	std::string operator;
	std::unique_ptr<ConditionalExpression> lhs;
	std::unique_ptr<ConditionalExpression> lhs;

	void accept(ASTVisitor* visitor) const override;
};