#pragma once

#include <memory>
#include "../Visitor/ASTVisitor.h"

// interface for ASTNodes 
class ASTNode {
private:
	int lineNumber;
public:
	virtual void accept(std::shared_ptr<ASTVisitor> visitor) = 0;
};