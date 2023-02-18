#pragma once

#include "Statement.h"

class CallStatement : public Statement {
public:
	std::string procName;

	CallStatement();
	CallStatement(std::string procName);

	void accept(ASTVisitor* visitor) override;
};