#pragma once

#include <string>
#include "Statement.h"

class ReadStatement : public Statement {
public:
	std::string varName;

	ReadStatement();

	ReadStatement(std::string varName);

	void accept(ASTVisitor* visitor) const override;
};