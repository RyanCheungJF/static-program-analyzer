#pragma once

#include "Statement.h"
#include "Variable.h"
#include "../Visitor/ASTVisitor.h"

class PrintStatement : public Statement {
public:
	std::string varName;
	//int lineNumber;

	PrintStatement();

	PrintStatement(std::string varName);

	void accept(ASTVisitor* visitor) const override;
};