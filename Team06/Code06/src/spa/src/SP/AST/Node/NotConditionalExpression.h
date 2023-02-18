#pragma once

#include "ConditionalExpression.h"
#include "ASTNode.h"

class NotConditionalExpression : public ConditionalExpression {
public:
	std::string op = "!";
	std::unique_ptr<ConditionalExpression> condExpr;

	NotConditionalExpression();
	NotConditionalExpression(std::unique_ptr<ConditionalExpression> condExpr);

	void accept(ASTVisitor* visitor) override;
};