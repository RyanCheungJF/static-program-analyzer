#pragma once

#include "Expression.h"

class ConditionalExpression : public ASTNode {
	void ConditionalExpression::accept(ASTVisitor* visitor) override {
		visitor->visitConditionalExpression(this);
	}
};