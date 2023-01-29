#pragma once

#include "Statement.h"
#include "Variable.h"
#include "../Visitor/ASTVisitor.h"

class PrintStatement : public Statement {
public:
	//Variable variable;
	//int lineNumber;

	PrintStatement();

	//PrintStatement(Variable variable, int lineNumber);

	void accept(ASTVisitor* visitor) const override;
};