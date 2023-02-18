#pragma once

#include <iostream>
#include "Expression.h"

class Constant : public Expression {
public:
	Const value;

	Constant();
	Constant(Const value);

	void accept(ASTVisitor* visitor) override;
};