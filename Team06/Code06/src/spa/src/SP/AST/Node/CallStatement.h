#pragma once

#include "Statement.h"
#include "Procedure.h"
#include "../Visitor/ASTVisitor.h"

class CallStatement : public Statement {
public:
	Procedure procedure;
	int lineNumber;

	CallStatement() = default;

	CallStatement(Procedure procedure, int lineNumber);

	void accept(ASTVisitor* visitor) const override;
};