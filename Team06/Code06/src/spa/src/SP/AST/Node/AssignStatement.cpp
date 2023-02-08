#include "AssignStatement.h"

AssignStatement::AssignStatement() {}

AssignStatement::AssignStatement(std::string varName, std::unique_ptr<Expression> expr) {
	this->varName = varName;
	this->expr = std::move(expr);
}

void AssignStatement::accept(ASTVisitor* visitor) {
	visitor->visitAssignStatement(this);
}