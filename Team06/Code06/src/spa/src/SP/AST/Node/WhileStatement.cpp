#include "WhileStatement.h"

WhileStatement::WhileStatement() {};

WhileStatement::WhileStatement(std::unique_ptr<ConditionalExpression> condExpr, std::unique_ptr<StatementList> stmtList) {
	this->condExpr = std::move(condExpr);
	this->stmtList = std::move(stmtList);
}

void WhileStatement::accept(ASTVisitor* visitor) const {
	visitor->visitWhileStatement(this);
}