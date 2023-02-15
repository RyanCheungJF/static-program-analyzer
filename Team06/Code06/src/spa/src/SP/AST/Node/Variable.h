#pragma once

#include <iostream>
#include "Expression.h"

class Variable : public Expression {
public:
	std::string name;

	Variable();
	Variable(std::string name);

	void accept(ASTVisitor* visitor) override;
};