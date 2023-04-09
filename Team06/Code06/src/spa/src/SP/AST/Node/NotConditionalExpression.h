#pragma once

#include "ASTNode.h"
#include "ConditionalExpression.h"

class NotConditionalExpression : public ConditionalExpression {
public:
    Operator op = AppConstants::STRING_NOT;
    std::unique_ptr<ConditionalExpression> condExpr;

    NotConditionalExpression();
    explicit NotConditionalExpression(std::unique_ptr<ConditionalExpression> condExpr);
};