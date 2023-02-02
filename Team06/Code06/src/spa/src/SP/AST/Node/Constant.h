#pragma once

#include <iostream>
#include "Expression.h"

class Constant : public Expression {
public:
	int value;

	void accept(ASTVisitor* visitor) const override;
};