#pragma once

#include "Statement.h"
#include <string>

class CallStatement : public Statement {
public:
	std::string procName;
	//int lineNumber;

	void accept(ASTVisitor* visitor) const override;
};