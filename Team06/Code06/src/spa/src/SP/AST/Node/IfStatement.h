#pragma once

#include "ConditionalExpression.h"
#include "StatementList.h"
#include "../Visitor/ASTVisitor.h"

class IfStatement : public Statement {
public:
	std::unique_ptr<ConditionalExpression> condExpr;
	std::unique_ptr<StatementList> thenStmtList;
	std::unique_ptr<StatementList> elseStmtList;
	//int lineNumber;

	IfStatement();

	//IfStatement(ConditionalExpression conditional, StatementList thenstatementList, StatementList elseStatementList, int lineNumber);

	void accept(ASTVisitor* visitor) const override;
};