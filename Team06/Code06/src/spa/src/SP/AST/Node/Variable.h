#pragma once

#include <iostream>
#include "Expression.h"

class Variable : public Expression {
public:
	std::string variableName;

	void accept(ASTVisitor *visitor) const override;
};