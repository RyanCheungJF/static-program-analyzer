#include "IfStatement.h"

//IfStatement::IfStatement(ConditionalExpression conditional, StatementList thenStatementList, StatementList elseStatementList, int lineNumber) {
//	this->conditional = conditional;
//	this->thenStatementList = thenstatementList;
//	this->elseStatementList = elseStatementList;
//	this->lineNumber = lineNumber;
//}

IfStatement::IfStatement() {}

void IfStatement::accept(ASTVisitor* visitor) const {
	visitor->visitIfStatement(this);
}