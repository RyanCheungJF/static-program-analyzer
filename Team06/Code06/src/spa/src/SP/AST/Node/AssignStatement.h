#pragma once

#include "Statement.h"
#include "Variable.h"
#include "../Visitor/ASTVisitor.h"

class AssignStatement : public Statement {
public:
	Variable lhs;
	Statement rhs;
	int lineNumber;

	AssignStatement() = default;

	AssignStatement(Variable lhs, Statement rhs, int lineNumber);

	void accept(ASTVisitor* visitor) const override;
};