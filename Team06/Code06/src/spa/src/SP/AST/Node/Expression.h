#pragma once

#include "ASTNode.h"

class Expression : public ASTNode {
    void accept(ASTVisitor* visitor) override {
        visitor->visitExpression(this);
    }
};