#include "BinaryConditionalExpression.h"

BinaryConditionalExpression::BinaryConditionalExpression() : condOperator(), lhs(), rhs() {}

BinaryConditionalExpression::BinaryConditionalExpression(Operator condOperator,
                                                         std::unique_ptr<ConditionalExpression> lhs,
                                                         std::unique_ptr<ConditionalExpression> rhs) {
    this->condOperator = condOperator;
    this->lhs = std::move(lhs);
    this->rhs = std::move(rhs);
}