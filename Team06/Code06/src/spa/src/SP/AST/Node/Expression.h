#pragma once

#include "ASTNode.h"

class Expression : public ASTNode {
	void Expression::accept(ASTVisitor* visitor) override {
		visitor->visitExpression(this);
	}
};