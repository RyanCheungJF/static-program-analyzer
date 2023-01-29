#pragma once

#include <iostream>
#include "ASTNode.h"

class Variable : public ASTNode {
public:
	std::string variableName;
	std::string value;

	Variable();

	Variable(std::string variableName, std::string value);

	void accept(ASTVisitor *visitor) const override;
};