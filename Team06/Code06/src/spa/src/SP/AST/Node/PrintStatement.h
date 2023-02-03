#pragma once

#include <string>
#include "Statement.h"

class PrintStatement : public Statement {
public:
	std::string varName;
	//int lineNumber;

	PrintStatement();

	PrintStatement(std::string varName);

	void accept(ASTVisitor* visitor) const override;
};