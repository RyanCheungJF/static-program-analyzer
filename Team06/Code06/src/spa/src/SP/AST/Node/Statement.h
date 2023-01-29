#pragma once

#include "ASTNode.h"

class Statement : public ASTNode {
public:
	//int lineNumber;

	Statement();

	void accept(ASTVisitor* visitor) const override;
};