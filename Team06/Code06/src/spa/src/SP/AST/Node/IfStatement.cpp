#include "IfStatement.h"

IfStatement::IfStatement() {};

IfStatement::IfStatement(StmtNum stmtNum, std::unique_ptr<ConditionalExpression> condExpr,
	std::unique_ptr<StatementList> thenStmtList,
	std::unique_ptr<StatementList> elseStmtList) {
	this->statementNumber = stmtNum;
	this->condExpr = std::move(condExpr);
	this->thenStmtList = std::move(thenStmtList);
	this->elseStmtList = std::move(elseStmtList);
}

void IfStatement::accept(ASTVisitor* visitor) {
	visitor->visitIfStatement(this);
}