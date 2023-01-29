#pragma once

#include "Statement.h"
#include "Variable.h"
#include "../Visitor/ASTVisitor.h"

class AssignStatement : public Statement {
public:
	//Variable lhs;
	//Statement rhs;
	//int lineNumber;

	AssignStatement();

	void accept(ASTVisitor* visitor) const override;
};