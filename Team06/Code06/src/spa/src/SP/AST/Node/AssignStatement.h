#pragma once

#include <string>
#include <memory>
#include "Statement.h"
#include "Expression.h"

class AssignStatement : public Statement {
public:
	std::string varName;
	std::unique_ptr<Expression> expr;
	//int lineNumber;

	AssignStatement();

	AssignStatement(std::string varName, std::unique_ptr<Expression> expr);

	void accept(ASTVisitor* visitor) const override;
};