#include "RelationalExpression.h"

RelationalExpression::RelationalExpression() : relationalOperator(), lhs(), rhs(){};

RelationalExpression::RelationalExpression(Operator relationalOperator, std::unique_ptr<Expression> lhs,
                                           std::unique_ptr<Expression> rhs) {
    this->relationalOperator = relationalOperator;
    this->lhs = std::move(lhs);
    this->rhs = std::move(rhs);
}
