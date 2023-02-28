#include "NotConditionalExpression.h"

NotConditionalExpression::NotConditionalExpression() {}

NotConditionalExpression::NotConditionalExpression(
    std::unique_ptr<ConditionalExpression> condExpr) {
  this->condExpr = std::move(condExpr);
}

void NotConditionalExpression::accept(ASTVisitor *visitor) {
  visitor->visitConditionalExpression(this);
}