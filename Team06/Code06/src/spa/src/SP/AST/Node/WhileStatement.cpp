#include "WhileStatement.h"

WhileStatement::WhileStatement(ConditionalExpression conditional, StatementList statementList, int lineNumber) {
	this->conditional = conditional;
	this->statementList = statementList;
	this->lineNumber = lineNumber;
};

WhileStatement::accept(ASTVisitor* visitor) const {
	visitor->visitWhileStatement(this);
}