#pragma once

#include <iostream>
#include "ASTNode.h"

class Constant : public ASTNode {
public:
	//std::string variableName;
	//int value;

	Constant();

	//Constant(std::string variableName, int value);

	void accept(ASTVisitor* visitor) const override;
};