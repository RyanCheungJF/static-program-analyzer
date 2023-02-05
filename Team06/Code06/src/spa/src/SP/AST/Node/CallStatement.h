#pragma once

#include "Statement.h"
#include <string>

class CallStatement : public Statement {
public:
	std::string procName;

	void accept(ASTVisitor* visitor) const override;
};