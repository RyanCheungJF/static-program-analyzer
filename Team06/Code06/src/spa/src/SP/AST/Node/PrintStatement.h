#pragma once

#include <string>
#include "Statement.h"

class PrintStatement : public Statement {
public:
	std::string varName;
	//int lineNumber;

	void accept(ASTVisitor* visitor) const override;
};