#pragma once

#include "Statement.h"
#include "Variable.h"
//#include "../Visitor/ASTVisitor.h"

class ReadStatement : public Statement {
public:
	std::string varName;
	//int lineNumber;

	ReadStatement();

	ReadStatement(std::string varName);

	void accept(ASTVisitor* visitor) const override;
};