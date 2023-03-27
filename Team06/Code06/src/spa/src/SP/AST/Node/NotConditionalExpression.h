#pragma once

#include "ASTNode.h"
#include "ConditionalExpression.h"

class NotConditionalExpression : public ConditionalExpression {
public:
    Operator op = AppConstants::STRING_NOT;
    std::unique_ptr<ConditionalExpression> condExpr;

    NotConditionalExpression();
    NotConditionalExpression(std::unique_ptr<ConditionalExpression> condExpr);
};