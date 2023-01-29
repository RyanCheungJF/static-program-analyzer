#pragma once

#include "ConditionalExpression.h"
#include "StatementList.h"
#include "../Visitor/ASTVisitor.h"

class WhileStatement : public Statement {
public:
	std::unique_ptr<ConditionalExpression> condExpr;
	std::unique_ptr<StatementList> stmtList;
	//int lineNumber;

	WhileStatement();

	void accept(ASTVisitor* visitor) const override;
};