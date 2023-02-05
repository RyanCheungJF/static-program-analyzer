#pragma once

#include "../Visitor/ASTVisitor.h"

// interface for ASTNodes 
class ASTNode {
public:
	virtual void accept(ASTVisitor* visitor) const = 0;
};