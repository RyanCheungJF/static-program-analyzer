#include "MathExpression.h"

MathExpression::MathExpression() : mathOperator(), lhs(), rhs() {}

MathExpression::MathExpression(Operator mathOperator, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs)
    : mathOperator(std::move(mathOperator)), lhs(std::move(lhs)), rhs(std::move(rhs)) {}