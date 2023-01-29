//#pragma once
//
//#include "ConditionalExpression.h"
//#include "StatementList.h"
//#include "../Visitor/ASTVisitor.h"
//
//class WhileStatement : public Statement {
//public:
//	ConditionalExpression conditional;
//	StatementList statementList;
//	int lineNumber;
//
//	WhileStatement(ConditionalExpression conditional, StatementList statementList, int lineNumber);
//
//	void accept(ASTVisitor visitor);
//};