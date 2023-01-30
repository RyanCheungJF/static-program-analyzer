#pragma once

#include "Statement.h"
#include <string>
#include "../Visitor/ASTVisitor.h"

class CallStatement : public Statement {
public:
	std::string procName;
	//int lineNumber;

	CallStatement();

	void accept(ASTVisitor* visitor) const override;
};