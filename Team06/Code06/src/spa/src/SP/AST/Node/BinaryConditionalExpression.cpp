#include "BinaryConditionalExpression.h"

BinaryConditionalExpression::BinaryConditionalExpression() : condOperator(), lhs(), rhs() {}

BinaryConditionalExpression::BinaryConditionalExpression(Operator condOperator,
                                                         std::unique_ptr<ConditionalExpression> lhs,
                                                         std::unique_ptr<ConditionalExpression> rhs)
    : condOperator(std::move(condOperator)), lhs(std::move(lhs)), rhs(std::move(rhs)) {}