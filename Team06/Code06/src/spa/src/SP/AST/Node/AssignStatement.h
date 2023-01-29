#pragma once

#include "Statement.h"
#include "Variable.h"
#include "Expression.h"
#include "../Visitor/ASTVisitor.h"

class AssignStatement : public Statement {
public:
	std::string varName;
	std::unique_ptr<Expression> expr;
	//Statement rhs;
	//int lineNumber;

	AssignStatement();

	void accept(ASTVisitor* visitor) const override;
};