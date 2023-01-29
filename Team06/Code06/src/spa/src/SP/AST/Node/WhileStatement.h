#pragma once

#include "ConditionalExpression.h"
#include "StatementList.h"
#include "../Visitor/ASTVisitor.h"

class WhileStatement : public Statement {
public:
	//ConditionalExpression conditional;
	//StatementList statementList;
	//int lineNumber;

	WhileStatement();

	void accept(ASTVisitor* visitor) const override;
};