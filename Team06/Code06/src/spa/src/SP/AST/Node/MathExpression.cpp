#include "MathExpression.h"

MathExpression::MathExpression() : mathOperator(), lhs(), rhs() {}

MathExpression::MathExpression(Operator mathOperator, std::unique_ptr<Expression> lhs,
                               std::unique_ptr<Expression> rhs) {
    this->mathOperator = mathOperator;
    this->lhs = std::move(lhs);
    this->rhs = std::move(rhs);
}