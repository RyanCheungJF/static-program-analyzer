#pragma once

#include <iostream>
#include "Expression.h"

class Variable : public Expression {
public:
	Ent name;

	Variable();
	Variable(Ent name);

	void accept(ASTVisitor* visitor) override;
};