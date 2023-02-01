#pragma once

#include <iostream>
#include "Expression.h"

class Variable : public Expression {
public:
	std::string name;

	Variable(std::string val);
	void accept(ASTVisitor* visitor) const override;
};