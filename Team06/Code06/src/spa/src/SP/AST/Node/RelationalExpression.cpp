#include "RelationalExpression.h"

RelationalExpression::RelationalExpression() : relationalOperator(), lhs(), rhs(){};

RelationalExpression::RelationalExpression(Operator relationalOperator, std::unique_ptr<Expression> lhs,
                                           std::unique_ptr<Expression> rhs)
    : relationalOperator(relationalOperator), lhs(std::move(lhs)), rhs(std::move(rhs)) {}
