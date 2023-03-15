#pragma once

#include <memory>
#include <string>

#include "../../../utils/AppConstants.h"
#include "../Visitor/ASTVisitor.h"

// interface for ASTNodes
class ASTNode {
public:
    virtual void accept(ASTVisitor* visitor) = 0;
};