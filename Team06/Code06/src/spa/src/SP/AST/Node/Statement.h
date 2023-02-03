#pragma once

#include "ASTNode.h"

class Statement : public ASTNode {
public:
    void accept(ASTVisitor* visitor) const override;
	int lineNumber;

};