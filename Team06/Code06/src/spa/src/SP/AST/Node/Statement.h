#pragma once

#include "ASTNode.h"

class Statement : public ASTNode {
public:
	//int lineNumber;

	Statement() = default;

	void accept(ASTVisitor* visitor) const override;
};