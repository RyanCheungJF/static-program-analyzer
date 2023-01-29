#pragma once

#include "Statement.h"
#include "Variable.h"
#include "../Visitor/ASTVisitor.h"

class ReadStatement : public Statement {
public:
	Variable variable;
	int lineNumber;

	ReadStatement() = default;

	ReadStatement(Variable variable, int lineNumber);

	void accept(ASTVisitor* visitor) const override;
};