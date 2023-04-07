#include "NotConditionalExpression.h"

NotConditionalExpression::NotConditionalExpression() : condExpr() {}

NotConditionalExpression::NotConditionalExpression(std::unique_ptr<ConditionalExpression> condExpr)
    : condExpr(std::move(condExpr)) {}