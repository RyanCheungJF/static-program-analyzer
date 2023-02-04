#pragma once

#include "ConditionalExpression.h"
#include "StatementList.h"

class IfStatement : public Statement {
public:
	std::unique_ptr<ConditionalExpression> condExpr;
	std::unique_ptr<StatementList> thenStmtList;
	std::unique_ptr<StatementList> elseStmtList;

	void accept(ASTVisitor* visitor) const override;
};