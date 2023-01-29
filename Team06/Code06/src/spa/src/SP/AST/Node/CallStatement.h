#pragma once

#include "Statement.h"
#include "Procedure.h"
#include "../Visitor/ASTVisitor.h"

class CallStatement : public Statement {
public:
	Procedure procedure;
	int lineNumber;

	CallStatement();

	void accept(ASTVisitor *visitor) const override;
};