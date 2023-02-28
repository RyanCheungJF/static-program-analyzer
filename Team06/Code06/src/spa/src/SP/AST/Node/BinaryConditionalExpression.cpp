#include "BinaryConditionalExpression.h"

BinaryConditionalExpression::BinaryConditionalExpression() {}

BinaryConditionalExpression::BinaryConditionalExpression(
    Operator condOperator, std::unique_ptr<ConditionalExpression> lhs,
    std::unique_ptr<ConditionalExpression> rhs) {
  this->condOperator = condOperator;
  this->lhs = std::move(lhs);
  this->rhs = std::move(rhs);
}

void BinaryConditionalExpression::accept(ASTVisitor *visitor) {
  visitor->visitConditionalExpression(this);
}