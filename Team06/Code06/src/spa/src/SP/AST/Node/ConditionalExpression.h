#pragma once

#include "Expression.h"

class ConditionalExpression : public ASTNode {
    void accept(ASTVisitor* visitor) override {
        visitor->visitConditionalExpression(this);
    }
};