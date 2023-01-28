#pragma once

#include "Statement.h"
#include "Variable.h"
#include "../Visitor/ASTVisitor.h"

class AssignStatement : public Statement {
public:
	Variable lhs;
	Statement rhs;
	int lineNumber;

	AssignStatement(Variable lhs, Statement rhs, int lineNumber);

	void accept(ASTVisitor visitor);
};