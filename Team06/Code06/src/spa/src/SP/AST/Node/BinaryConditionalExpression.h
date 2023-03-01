#pragma once

#include "ASTNode.h"
#include "ConditionalExpression.h"

class BinaryConditionalExpression : public ConditionalExpression {
public:
    Operator condOperator;
    std::unique_ptr<ConditionalExpression> lhs;
    std::unique_ptr<ConditionalExpression> rhs;

    BinaryConditionalExpression();

    BinaryConditionalExpression(Operator condOperator, std::unique_ptr<ConditionalExpression> lhs,
                                std::unique_ptr<ConditionalExpression> rhs);

    void accept(ASTVisitor* visitor) override;
};