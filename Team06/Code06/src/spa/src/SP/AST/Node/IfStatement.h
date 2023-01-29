#pragma once

#include "ConditionalExpression.h"
#include "StatementList.h"
#include "../Visitor/ASTVisitor.h"

class IfStatement : public Statement {
public:
	ConditionalExpression conditional;
	StatementList thenStatementList;
	StatementList elseStatementList;
	int lineNumber;

	IfStatement() = default;

	IfStatement(ConditionalExpression conditional, StatementList thenstatementList, StatementList elseStatementList, int lineNumber);

	void accept(ASTVisitor* visitor) const override;
};